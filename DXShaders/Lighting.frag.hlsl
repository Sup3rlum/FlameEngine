
#include "Utilities/CookTorrance.hlsl"
#include "Utilities/Spaces.hlsl"
#include "Utilities/LightData.hlsl"
#include "VXGI/VXGIBuffers.hlsl"

#define SM_CASCADES 5

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};


Texture2D gDepth : register(t0);
Texture2D gNormal : register(t1);
Texture2D gAlbedo : register(t2);
Texture2D gRoughnessMetallic : register(t3);
Texture2D gEmissive : register(t4);
Texture2D gHbao : register(t5);
Texture2DArray gShadowmap : register(t6);

Texture2D gBRDF_LUT : register(t7);
Texture3D gVoxelRadiance : register(t9);
Texture3D DirectionalVolumes[6] : register(t10);

SamplerState DepthSampler : register(s0);
SamplerState NormalSampler : register(s1);
SamplerState AlbedoSampler : register(s2);
SamplerState RMSampler : register(s3);
SamplerState EmissiveSampler : register(s4);
SamplerState HbaoSampler : register(s5);
SamplerState ShadowmapSampler : register(s6);
SamplerState BRDFSampler : register(s7);
SamplerState VoxelSampler : register(s9);



cbuffer LightingPassBuffer : register(b4)
{
    matrix View;
    matrix Projection;
    matrix InverseView;
    matrix InverseProjection;
    
    float4 CameraPosition; 
    float zNear;
    float zFar;
    float NumDLights;
    float NumPLights;
    float NumSLights;
}

cbuffer CascadeBuffer : register(b2)
{
    CascadeInfo SunCascades[SM_CASCADES];
}

#define MAX_LIGHTS 8

cbuffer DirectionalLightData : register(b6)
{
    DirectionalLight    DirectionalLights[MAX_LIGHTS];
}
cbuffer PointLightData : register(b7)
{
    PointLight          PointLights[MAX_LIGHTS];
}
cbuffer SpotLightData : register(b8)
{
    SpotLight           SpotLights[MAX_LIGHTS];
}

/*-------------*/


#define F0 0.04
#define PI 3.14159265f
#define HALF_PI 1.57079f

static const float3 diffuseConeDirections[] =
{
    float3(0.0f, 1.0f, 0.0f),
    float3(0.0f, 0.5f, 0.866025f),
    float3(0.823639f, 0.5f, 0.267617f),
    float3(0.509037f, 0.5f, -0.7006629f),
    float3(-0.50937f, 0.5f, -0.7006629f),
    float3(-0.823639f, 0.5f, 0.267617f)
};

static const float diffuseConeWeights[] =
{
    PI / 4.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
};


/* ------------------------------------------------------------------------------------ */

float3 UnpackNormal(float2 tCoord)
{
    return normalize(gNormal.Sample(NormalSampler, tCoord).rgb * 2 - 1.0f);
}

float GetDepth(float2 tCoord)
{
    return gDepth.Sample(DepthSampler, tCoord).r;
}

float GetLinearDepth(float2 tCoord)
{
    return LinearizeDepth(gDepth.Sample(DepthSampler, tCoord).r, zNear, zFar);
}


float GetLinearDepthTexelRaw(int2 pixel)
{
    return LinearizeDepth(gDepth.Load(int3(pixel, 0)).r, zNear, zFar);
}


float4 UnpackPosition(float2 tCoord)
{ 
    float z = GetDepth(tCoord);
    
    float x = tCoord.x * 2.0f - 1.0f;
    float y = (1 - tCoord.y) * 2 - 1.0f;
    
    float4 tRay = mul(InverseProjection, float4(x, y, z, 1.0f));
    return float4(tRay.xyz / tRay.w, 1.0f);
}

float VarianceShadow(float2 moments, float compare)
{
    
    float fAvgZ = moments.x;
    float fAvgZ2 = moments.y;
    
    if (compare <= fAvgZ)
    {
        return 1;
    }
    else
    {
        float variance = fAvgZ2 - (fAvgZ * fAvgZ);
        variance = max(variance, 0.00001f);

        float d = compare - fAvgZ;
        float pMax = smoothstep(0.0, 1.0, variance / (variance + d * d));

        return pow(pMax, 4.0);
    }
}

float ShadowCalculation(float4 fragPosLightSpace, float cascadeIndex)
{
    float4 shadowMapCoord = fragPosLightSpace / fragPosLightSpace.w;
    shadowMapCoord.y = -shadowMapCoord.y;
    float2 texCoords = shadowMapCoord.xy * 0.5 + 0.5;

    float depthCompare = shadowMapCoord.z;
    float2 moments = gShadowmap.Sample(ShadowmapSampler, float3(texCoords, (float)cascadeIndex)).xy;

    return VarianceShadow(moments, depthCompare);
}



/*********************************************************/


float4 AnistropicSample(float3 coord, float3 weight, uint3 face, float lod)
{
    // anisotropic volumes level
    float anisoLevel = max(lod - 1.0f, 0.0f);
    
    // X-axis Sample
    float4 aSampleX0 = DirectionalVolumes[0].SampleLevel(VoxelSampler, coord, anisoLevel);
    float4 aSampleX1 = DirectionalVolumes[1].SampleLevel(VoxelSampler, coord, anisoLevel);
    
    // Y-axis Sample
    float4 aSampleY0 = DirectionalVolumes[2].SampleLevel(VoxelSampler, coord, anisoLevel);
    float4 aSampleY1 = DirectionalVolumes[3].SampleLevel(VoxelSampler, coord, anisoLevel);
    
    // Z-axis Sample
    float4 aSampleZ0 = DirectionalVolumes[4].SampleLevel(VoxelSampler, coord, anisoLevel);
    float4 aSampleZ1 = DirectionalVolumes[5].SampleLevel(VoxelSampler, coord, anisoLevel);
    
    // Choose direction
    float4 aSampleX = lerp(aSampleX0, aSampleX1, face.x);
    float4 aSampleY = lerp(aSampleY0, aSampleY1, face.y);
    float4 aSampleZ = lerp(aSampleZ0, aSampleZ1, face.z);

    
    // Directional sample
    float4 anisoSample  = weight.x * aSampleX
                        + weight.y * aSampleY
                        + weight.z * aSampleZ;
    
    // linearly interpolate on base level
    if (lod < 1.0f)
    {
        float4 baseColor = gVoxelRadiance.Sample(VoxelSampler, coord);
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
    
    traceOcclusion = traceOcclusion && aoAlpha < 1.0f;

    float voxelWorldSize = 2.0 / (voxelScale * voxelVolumeDimension);
    float3 weight = direction * direction;
    float dst = voxelWorldSize;
    float3 startPosition = position + normal * dst;
    float4 coneSample = 0.0f;
    float occlusion = 0.0f;
    float maxDistance = maxTracingDistanceGlobal / voxelScale;
    float falloff = 0.5f * aoFalloff * voxelScale;
    float enter = 0.0;
    float leave = 0.0;


    [loop]
    while (coneSample.a < 1.0f && dst <= maxDistance)
    {
        float3 conePosition = startPosition + direction * dst;
        float diameter = 2.0f * aperture * dst;
        float mipLevel = log2(diameter / voxelWorldSize);
        float3 coord = float3(WorldToVoxel(conePosition));
        float4 anisoSample = AnistropicSample(coord, weight, visibleFace, mipLevel);
        coneSample += (1.0f - coneSample.a) * anisoSample;
        
        // ambient occlusion
        if (traceOcclusion && occlusion < 1.0)
        {
            occlusion += ((1.0f - occlusion) * anisoSample.a) / (1.0f + falloff * diameter);
        }
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

    uint3 visibleFace = direction >= 0;
    float voxelWorldSize = 2.0 / (voxelScale * voxelVolumeDimension);
    float3 weight = direction * direction;
    float dst = voxelWorldSize;
    float3 startPosition = position + direction * dst;
    float mipMaxLevel = log2(voxelVolumeDimension) - 1.0f;
    float visibility = 0.0f;
    float k = exp2(7.0f * coneShadowTolerance);
    float maxDistance = maxTracingDistance;
    float enter = 0.0;
    float leave = 0.0;
    
   
    [loop]
    while (visibility < 1.0f && dst <= maxDistance)
    {
        float3 conePosition = startPosition + direction * dst;
        float diameter = 2.0f * aperture * dst;
        float mipLevel = log2(diameter / voxelWorldSize);
        float3 coord = WorldToVoxel(conePosition);
        float4 anisoSample = AnistropicSample(coord, weight, visibleFace, mipLevel);

        if (hardShadows && anisoSample.a > 1e-30)
        {
            return 0.0f;
        }
        
        // accumulate
        visibility += (1.0f - visibility) * anisoSample.a * k;
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
    if (any(specular.rgb > specularTrace.rgb))
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

        [loop]
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



/*******************************************************/



float3 CalcPointLight(PointLight pLight, float3 FragPos, float3 Normal, float3 ViewDir, float3 Albedo, float Roughness, float Metallic, float3 Fresnel0)
{  
    float3 LightPosViewSpace = mul(View, float4(pLight.Position.xyz, 1.0)).xyz;
    float3 LightDir = normalize(LightPosViewSpace - FragPos);
    
    // Calculate point light attenuation
    float distance      = length(LightPosViewSpace - FragPos);
    float attDenom      = (distance / max(pLight.Radius, 0.001) + 1.0);
    float attenuation   = pLight.Radiance.a / (attDenom * attDenom);
    float3 radianceIn   = pLight.Radiance.rgb * attenuation;
    
    //float4 wsPosition = mul(InverseView, float4(FragPos, 1.0));
    //float shadow = max(0.0f, TraceShadowCone(wsPosition.xyz, LightDir, coneShadowAperture, distance));
    
    // Multiply radiance coeff by BRDF factor
    float3 radianceOut = BRDF(FragPos, Normal, ViewDir, LightDir, Albedo, Roughness, Metallic, Fresnel0) * radianceIn;
    return radianceOut;
}

float3 CalcSpotLight(SpotLight sLight, float3 FragPos, float3 Normal, float3 ViewDir, float3 Albedo, float Roughness, float Metallic, float3 Fresnel0)
{
    float3 vsLightPosition = mul(View, float4(sLight.Position.xyz, 1.0)).xyz;
    float3 vsLightDirection = mul((float3x3)View, sLight.Direction.xyz);
    float3 LightDir = normalize(vsLightPosition - FragPos);
    
    // Calculate spot light aperture incidence
    float DdotL = dot(vsLightDirection, -LightDir);
    float apertureFactor = DdotL >= sLight.ApertureSize;
    
    // Calculate point light attenuation
    float distance = length(vsLightPosition - FragPos);
    float attDenom = (distance / max(sLight.Radius, 0.001) + 1.0);
    float attenuation = sLight.Radiance.a / (attDenom * attDenom);
    float3 radianceIn = sLight.Radiance.rgb * attenuation * apertureFactor;
   
    
    // Multiply radiance coeff by BRDF factor
    float3 radianceOut = BRDF(FragPos, Normal, ViewDir, LightDir, Albedo, Roughness, Metallic, Fresnel0) * radianceIn;
    return radianceOut;
}



float3 CalcDirectionalLight(DirectionalLight dLight, float3 FragPos, float3 Normal, float3 ViewDir, float3 Albedo, float Roughness, float Metallic, float3 Fresnel0)
{
    
    int cascadeIdx = 0;
    float fDepth = FragPos.z;
    
    for (int i = 0; i < SM_CASCADES; ++i)
    {
        float cascadeViewDepthFar = SunCascades[i].MaxDepth.x;
        
        if (fDepth < cascadeViewDepthFar)
        {
            cascadeIdx = i;
            break;
        }
    }
    
    float4 wsPosition = mul(InverseView, float4(FragPos, 1.0));
    float4 lsVPosition = mul(SunCascades[cascadeIdx].LView, wsPosition);
    float4 lsPosition = mul(SunCascades[cascadeIdx].LProjection, lsVPosition);
    float Shadow = ShadowCalculation(lsPosition, cascadeIdx);
    //float Shadow = max(0.0f, TraceShadowCone(wsPosition.xyz, -dLight.Direction.xyz, coneShadowAperture, 1.0f / voxelScale));
     
    float3 LightDir = -normalize(mul((float3x3) View, dLight.Direction.xyz));  
    float3 radianceIn = dLight.Radiance.rgb * dLight.Radiance.a * Shadow;

    float3 radianceOut = BRDF(FragPos, Normal, ViewDir, LightDir, Albedo, Roughness, Metallic, Fresnel0) * radianceIn;       
    return radianceOut;
}


/*******************************************************/


float4 main(PSInput input) : SV_Target0
{
    
    float4 packedAlbedoOpacity      = gAlbedo.Sample(AlbedoSampler, input.TexCoord);
    float4 packedRoughnessMetallic  = gRoughnessMetallic.Sample(RMSampler, input.TexCoord);
    
    // Get PBR Map values
    float4 Position     = UnpackPosition(input.TexCoord);
    float3 Normal       = UnpackNormal(input.TexCoord);
    float Roughness     = packedRoughnessMetallic.r;
    float Metallic      = packedRoughnessMetallic.g;
    float MaterialAO    = packedRoughnessMetallic.b;
    
    float3 Albedo = packedAlbedoOpacity.rgb;
    float Opacity = packedAlbedoOpacity.a;
    
    float4 Emissive         = gEmissive.Sample(EmissiveSampler, input.TexCoord);
    float AmbientOcclusion  = gHbao.Sample(HbaoSampler, input.TexCoord).r;
   
    
    // Use a base fresnel reflectance based on metallicity
    float3 Fresnel0 = lerp(F0, Albedo, Metallic);
    
    
    // reflectance equation
    float3 ViewDir = -normalize(Position.xyz);
    
    
    /*--- Calculate Lights ---*/
    
    float3 directRadiance = 0;
    
    for (int i = 0; i < NumDLights; i++)
    {
        directRadiance += CalcDirectionalLight(DirectionalLights[i], Position.xyz, Normal, ViewDir, Albedo, Roughness, Metallic, Fresnel0);
    }

    for (int j = 0; j < NumPLights; j++)
    {
        directRadiance += CalcPointLight(PointLights[j], Position.xyz, Normal, ViewDir, Albedo, Roughness, Metallic, Fresnel0);
    }
    
    for (int k = 0; k < NumSLights; k++)
    {
        directRadiance += CalcSpotLight(SpotLights[k], Position.xyz, Normal, ViewDir, Albedo, Roughness, Metallic, Fresnel0);
    }
    
    /*--- VXGI ---*/
    
    float4 wsPosition = mul(InverseView, Position);  
    float3 wsNormal = mul(transpose((float3x3) View), Normal);
    float4 vxgi = float4(0, 0, 0, 1);// CalculateIndirectLighting(wsPosition.xyz, wsNormal, Albedo, float4(Albedo, 1 - Roughness), true);
    
    float3 vxgiAmbient = Albedo * 0.4f;
    vxgiAmbient *= 1.0f;// float3(0.83f, 0.86f, 1.0f);

    float vxgiAO = lerp(0.3f, 1.0f, AmbientOcclusion);
    
    /*--- Finalize ---*/
    
    //float3 FinalColor = gVoxelRadiance.Sample(VoxelSampler, WorldToVoxel(wsPosition.xyz));
    float3 FinalColor = (vxgiAmbient + directRadiance) * vxgiAO + Emissive.rgb * Emissive.a;


    return float4(FinalColor, Opacity);

}
