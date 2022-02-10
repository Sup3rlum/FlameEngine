

#include "VXGIBuffers.hlsl"

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
    float4 WorldPosition : WORLDPOS0;
	
};


#define PI 3.14159265f
#define HALF_PI 1.57079f

static float3 diffuseConeDirections[] =
{
    float3(0.0f, 1.0f, 0.0f),
    float3(0.0f, 0.5f, 0.866025f),
    float3(0.823639f, 0.5f, 0.267617f),
    float3(0.509037f, 0.5f, -0.7006629f),
    float3(-0.50937f, 0.5f, -0.7006629f),
    float3(-0.823639f, 0.5f, 0.267617f)
};

static float diffuseConeWeights[] =
{
    PI / 4.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
};

Texture2D DiffuseMap : register(t0);
SamplerState DiffuseSampler : register(s0);

Texture3D VoxelMap : register(t6);
SamplerState VoxelSampler : register(s6);


Texture3D DirectionalVolumes[6] : register(t7);

cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
    matrix InverseView;
    float4 CameraPosition;
};


cbuffer MaterialPropertiesBuffer : register(b5)
{
    float4 Emissive;
    
    uint HasNormalMap;
    uint HasPOMEnabled;
    uint HasAOMap;
    uint HasEmissive;
    uint HasTransluscent;
}


struct PointLight
{
    float4 Position;
    float4 Color;
    float Intensity;
    float Range;
};

cbuffer LightBuffer : register(b6)
{
    PointLight PointLights[4];
}


bool IsInCube(float3 pos)
{
    bool3 sides = pos <= 1.0 && pos >= 0.0;
    return all(sides);
}


float4 AnistropicSample(float3 coord, float3 weight, uint3 face, float lod)
{
    // anisotropic volumes level
    float anisoLevel = max(lod - 1.0f, 0.0f);
    
    
    float4 aSampleX0 = DirectionalVolumes[0].SampleLevel(VoxelSampler, coord, anisoLevel);
    float4 aSampleX1 = DirectionalVolumes[1].SampleLevel(VoxelSampler, coord, anisoLevel);
    

    float4 aSampleY0 = DirectionalVolumes[2].SampleLevel(VoxelSampler, coord, anisoLevel);
    float4 aSampleY1 = DirectionalVolumes[3].SampleLevel(VoxelSampler, coord, anisoLevel);
    

    float4 aSampleZ0 = DirectionalVolumes[4].SampleLevel(VoxelSampler, coord, anisoLevel);
    float4 aSampleZ1 = DirectionalVolumes[5].SampleLevel(VoxelSampler, coord, anisoLevel);
    
    
    float4 aSampleX = lerp(aSampleX0, aSampleX1, face.x);
    float4 aSampleY = lerp(aSampleY0, aSampleY1, face.y);
    float4 aSampleZ = lerp(aSampleZ0, aSampleZ1, face.z);

    
    
    // directional sample
    float4 anisoSample = weight.x * aSampleX
                     + weight.y * aSampleY
                     + weight.z * aSampleZ;
    // linearly interpolate on base level
    if (lod < 1.0f)
    {
        float4 baseColor = VoxelMap.Sample(VoxelSampler, coord);
        anisoSample = lerp(baseColor, anisoSample, clamp(lod, 0.0f, 1.0f));
    }

    return anisoSample;
}

bool IntersectRayWithWorldAABB(float3 ro, float3 rd, out float enter, out float leave)
{
    
    float3 minPosition = floor(CameraPosition.xyz) - sceneDimension / 2.0f;   
    float3 maxPosition = floor(CameraPosition.xyz) + sceneDimension / 2.0f;
    
    
    float3 tempMin = (minPosition - ro) / rd;
    float3 tempMax = (maxPosition - ro) / rd;
    
    float3 v3Max = max(tempMax, tempMin);
    float3 v3Min = min(tempMax, tempMin);
    
    leave = min(v3Max.x, min(v3Max.y, v3Max.z));
    enter = max(max(v3Min.x, 0.0), max(v3Min.y, v3Min.z));
    
    return leave > enter;
}


float4 TraceCone(float3 position, float3 normal, float3 direction, float aperture, bool traceOcclusion)
{
    uint3 visibleFace = direction >= 0;
    /*visibleFace.x = (direction.x < 0.0) ? 0 : 1;
    visibleFace.y = (direction.y < 0.0) ? 2 : 3;
    visibleFace.z = (direction.z < 0.0) ? 4 : 5;*/
    
    
    traceOcclusion = traceOcclusion && aoAlpha < 1.0f;
    // world space grid voxel size
    float voxelWorldSize = 2.0 / (voxelScale * voxelVolumeDimension);
    // weight per axis for aniso sampling
    float3 weight = direction * direction;
    // move further to avoid self collision
    float dst = voxelWorldSize;
    float3 startPosition = position + normal * dst;
    // final results
    float4 coneSample = 0.0f;
    float occlusion = 0.0f;
    float maxDistance = maxTracingDistanceGlobal / voxelScale;
    float falloff = 0.5f * aoFalloff * voxelScale;
    // out of boundaries check
    float enter = 0.0;
    float leave = 0.0;

    /*
    if (!IntersectRayWithWorldAABB(position, direction, enter, leave))
    {
        coneSample.a = 1.0f;
    }*/

    [loop]
    while (coneSample.a < 1.0f && dst <= maxDistance)
    {
        float3 conePosition = startPosition + direction * dst;
        // cone expansion and respective mip level based on diameter
        float diameter = 2.0f * aperture * dst;
        float mipLevel = log2(diameter / voxelWorldSize);
        // convert position to texture coord
        float3 coord = float3(WorldToVoxel(conePosition));
        // get directional sample from anisotropic representation
        float4 anisoSample = AnistropicSample(coord, weight, visibleFace, mipLevel);
        // front to back composition
        coneSample += (1.0f - coneSample.a) * anisoSample;
        // ambient occlusion
        if (traceOcclusion && occlusion < 1.0)
        {
            occlusion += ((1.0f - occlusion) * anisoSample.a) / (1.0f + falloff * diameter);
        }
        // move further into volume
        dst += diameter * samplingFactor;

    }

    return float4(coneSample.rgb, occlusion);
}


float TraceShadowCone(float3 position, float3 direction, float aperture, float maxTracingDistance)
{
    bool hardShadows = false;

    if (coneShadowTolerance == 1.0f)
    {
        hardShadows = true;
    }

    // directional dominat axis
    uint3 visibleFace = direction >= 0;

    // world space grid size
    float voxelWorldSize = 2.0 / (voxelScale * voxelVolumeDimension);
    // weight per axis for aniso sampling
    float3 weight = direction * direction;
    // move further to avoid self collision
    float dst = voxelWorldSize;
    float3 startPosition = position + direction * dst;
    // control vars
    float mipMaxLevel = log2(voxelVolumeDimension) - 1.0f;
    // final results
    float visibility = 0.0f;
    float k = exp2(7.0f * coneShadowTolerance);
    // cone will only trace the needed distance
    float maxDistance = maxTracingDistance;
    // out of boundaries check
    float enter = 0.0;
    float leave = 0.0;

    /*
    if (!IntersectRayWithWorldAABB(position, direction, enter, leave))
    {
        visibility = 1.0f;
    }*/
    
    [loop]
    while (visibility < 1.0f && dst <= maxDistance)
    {
        float3 conePosition = startPosition + direction * dst;
        float diameter = 2.0f * aperture * dst;
        float mipLevel = log2(diameter / voxelWorldSize);
        // convert position to texture coord
        float3 coord = WorldToVoxel(conePosition);
        // get directional sample from anisotropic representation
        float4 anisoSample = AnistropicSample(coord, weight, visibleFace, mipLevel);

        // hard shadows exit as soon cone hits something
        if (hardShadows && anisoSample.a > 1e-30)
        {
            return 0.0f;
        }
        // accumulate
        visibility += (1.0f - visibility) * anisoSample.a * k;
        // move further into volume
        dst += diameter * samplingFactor;
        
    }

    return 1.0f - visibility;
}

float4 CalculateIndirectLighting(float3 position, float3 normal, float3 albedo, float4 specular, bool ambientOcclusion)
{
    float4 specularTrace = 0.0f;
    float4 diffuseTrace = 0.0f;
    float3 coneDirection = 0.0f;

    // component greater than zero
    if (any(specular.rgb >specularTrace.rgb))
    {
        float3 viewDirection = normalize(CameraPosition.xyz - position);
        float3 coneDirection = reflect(viewDirection, normal);
        coneDirection = normalize(coneDirection);
        // specular cone setup, minimum of 1 grad, fewer can severly slow down performance
        float aperture = clamp(tan(HALF_PI * (1.0f - specular.a)), 0.0174533f, PI);
        specularTrace = TraceCone(position, normal, coneDirection, aperture, false);
        specularTrace.rgb *= specular.rgb;
    }

    // component greater than zero
    if (any(albedo > diffuseTrace.rgb))
    {
        // diffuse cone setup
        const float aperture = 0.57735f;
        float3 guide = float3(0.0f, 1.0f, 0.0f);

        if (abs(dot(normal, guide)) == 1.0f)
        {
            guide = float3(0.0f, 0.0f, 1.0f);
        }

        // Find a tangent and a bitangent
        float3 right = normalize(guide - dot(normal, guide) * normal);
        float3 up = cross(right, normal);

        for (int i = 0; i < 6; i++)
        {
            coneDirection = normal;
            coneDirection += diffuseConeDirections[i].x * right + diffuseConeDirections[i].z * up;
            coneDirection = normalize(coneDirection);
            // cumulative result
            diffuseTrace += TraceCone(position, normal, coneDirection, aperture, ambientOcclusion) * diffuseConeWeights[i];
        }

        diffuseTrace.rgb *= albedo;
    }

    float3 result = bounceStrength * (diffuseTrace.rgb + specularTrace.rgb);

    return float4(result, ambientOcclusion ? clamp(1.0f - diffuseTrace.a + aoAlpha, 0.0f, 1.0f) : 1.0f);
}

float3 CalculateDirectLighting(float3 pos, float3 normal, float3 albedo)
{
    float3 dirLight = -normalize(float3(-1, -1, -1));
    float diffuseFactor = max(dot(dirLight, normal), 0);
    float3 diffuseLight = albedo * diffuseFactor;
    
    float visibility = 1;//max(0.0f, TraceShadowCone(pos, dirLight, coneShadowAperture, 1.0f / voxelScale));
    
    return diffuseLight * visibility;
}


float3 CalculatePointLighting(PointLight pl, float3 position, float3 normal, float3 albedo)
{
    
    float3 lightDirection = normalize(pl.Position.xyz - position);
    float NdotL = max(dot(lightDirection, normal), 0);
    
    float distance = length(pl.Position.xyz - position);
    float attDenom = ((distance / max(pl.Range, 0.01)) + 1);
    float attenuation = pl.Intensity / (attDenom * attDenom);
    float3 radiance = pl.Color.rgb * albedo * attenuation * NdotL;
    
    
    float visibility = 1;//max(0.0f, TraceShadowCone(position, lightDirection, coneShadowAperture, distance));
    
    return radiance * visibility;
}


float4 main(PSInput input) : SV_TARGET
{
   
    float4 albedo = float4(DiffuseMap.Sample(DiffuseSampler, input.TexCoord).rgb, 1.0);
    float3 worldNormal = mul(transpose((float3x3) View), input.Normal);
    
    float3 diffuseLight = CalculateDirectLighting(input.WorldPosition.xyz, worldNormal, albedo.rgb);
    
    for (int i = 0; i < 4; i++)
    {
        diffuseLight += CalculatePointLighting(PointLights[i], input.WorldPosition.xyz, worldNormal, albedo.rgb);
    }
    
    diffuseLight = 0;
    
    float4 indirLight = CalculateIndirectLighting(input.WorldPosition.xyz, worldNormal, albedo.rgb, float4(albedo.rgb, 0.5), true);
    float3 lighting = (diffuseLight + indirLight.rgb) * indirLight.a;
   
    
    return float4(lighting, 1.0);
}