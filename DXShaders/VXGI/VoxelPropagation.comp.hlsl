
#include "../Utilities/Packing.hlsl"
#include "VXGIBuffers.hlsl"

RWTexture3D<uint> g_VoxelAlbedo : register(u0);
RWTexture3D<uint> g_VoxelNormal : register(u1);
RWTexture3D<unorm float4> g_VoxelLit : register(u2);

RWTexture3D<unorm float4> g_VoxelFirstBounce : register(u3);

Texture3D g_VoxelTexMipmap[6] : register(t0);
SamplerState VoxelSampler : register(s0);



static const float PI = 3.14159265f;
static const float EPSILON = 1e-30;


// four cones
static const float3 propagationDirections[] =
{
    float3(0.0f, 1.0f, 0.0f),
    float3(0.0f, 0.5f, 0.866025f),
    float3(0.754996f, 0.5f, -0.4330128f),
    float3(-0.754996f, 0.5f, -0.4330128f)
};

static const float diffuseConeWeights[] =
{
    PI / 3.0f,
    2.0f * PI / 9.0f,
    2.0f * PI / 9.0f,
    2.0f * PI / 9.0f,
};



cbuffer PropagationProperties : register(b11)
{
    float BounceNumber;
    float _padding23[3];
}

float4 AnisotropicSample(float3 coord, float3 weight, uint3 face, float anisoLevel)
{


    float4 aSampleX0 = g_VoxelTexMipmap[0].SampleLevel(VoxelSampler, coord, anisoLevel);
    float4 aSampleX1 = g_VoxelTexMipmap[1].SampleLevel(VoxelSampler, coord, anisoLevel);
    

    float4 aSampleY0 = g_VoxelTexMipmap[2].SampleLevel(VoxelSampler, coord, anisoLevel);
    float4 aSampleY1 = g_VoxelTexMipmap[3].SampleLevel(VoxelSampler, coord, anisoLevel);
    

    float4 aSampleZ0 = g_VoxelTexMipmap[4].SampleLevel(VoxelSampler, coord, anisoLevel);
    float4 aSampleZ1 = g_VoxelTexMipmap[5].SampleLevel(VoxelSampler, coord, anisoLevel);
    
    
    float4 aSampleX = lerp(aSampleX0, aSampleX1, float(face.x));
    float4 aSampleY = lerp(aSampleY0, aSampleY1, float(face.y));
    float4 aSampleZ = lerp(aSampleZ0, aSampleZ1, float(face.z));

    
    
    // directional sample
    float4 anisoSample = weight.x * aSampleX
                     + weight.y * aSampleY
                     + weight.z * aSampleZ;

    return anisoSample;
}



float4 TraceCone(float3 position, float3 direction, float aperture)
{
    uint3 visibleFace = direction >= 0;
    // in propagation mode we only trace the mip levels for 
    // speed and to avoid aliasing issues
    float anisoDimension = voxelVolumeDimension / 2.0f;

    // weight per axis for aniso sampling
    float3 weight = direction * direction;
    // navigation
    float voxelSize = 1.0f / anisoDimension;
    // move one voxel further to avoid self collision
    float dst = voxelSize;
    float diameter = aperture * dst;
    float3 samplePos = position + direction * dst;
    // control vars
    float mipLevel = 0.0f;
    // accumulated sample
    float4 coneSample = 0.0f;
    float4 anisoSample = 0.0f;
    
    // outside bounds
    if (samplePos.x < 0.0f || samplePos.y < 0.0f || samplePos.z < 0.0f
        || samplePos.x > 1.0f || samplePos.y > 1.0f || samplePos.z > 1.0f)
    {
        return coneSample;
    }

    
    [loop]
    while (coneSample.a <= 1.0f && dst <= maxTracingDistanceGlobal)
    {
        // outisde bounds
        /*if (samplePos.x < 0.0f || samplePos.y < 0.0f || samplePos.z < 0.0f
            || samplePos.x > 1.0f || samplePos.y > 1.0f || samplePos.z > 1.0f)
        {
            break;
        }*/

        mipLevel = log2(diameter * anisoDimension);
        mipLevel = max(mipLevel - 1.0f, 0.0f);
        // aniso sampling
        anisoSample = AnisotropicSample(samplePos, weight, visibleFace, mipLevel);
        // accumulate sampling
        coneSample += (1.0f - coneSample.a) * anisoSample;
        // move further into volume
        dst += max(diameter, voxelSize);
        diameter = dst * aperture;
        samplePos = direction * dst + position;
        
    }

    return coneSample;
}

float4 CalculateIndirectLighting(float3 position, float3 normal)
{
    // move position further along normal, 1 voxel step, half dimension
    position = position + normal * (1.0f / (voxelVolumeDimension / 2.0f));
    // cummulative
    float4 diffuseTrace = 0.0f;
    // diffuse cone setup
    const float aperture = 1.0f;
    float3 guide = float3(0.0f, 1.0f, 0.0f);

    if (abs(dot(normal, guide)) == 1.0f)
    {
        guide = float3(0.0f, 0.0f, 1.0f);
    }

    // Find a tangent and a bitangent
    float3 right = normalize(guide - dot(normal, guide) * normal);
    float3 up = cross(right, normal);

    for (int i = 0; i < 4; i++)
    {
        float3 coneDirection = normal;
        coneDirection += propagationDirections[i].x * right + propagationDirections[i].z * up;
        coneDirection = normalize(coneDirection);

        diffuseTrace += TraceCone(position, coneDirection, aperture) * diffuseConeWeights[i];
    }

    return clamp(diffuseTrace, 0.0f, 1.0f) * 1.0 / BounceNumber;
}

float3 UnpackNormal(float3 pNormal)
{
    return normalize(pNormal * 2 - 1.0f);
}


[numthreads(8, 8, 8)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (DTid.x >= voxelVolumeDimension ||
        DTid.y >= voxelVolumeDimension ||
        DTid.z >= voxelVolumeDimension)
        return;

    //int3 writePos = int3(DTid);
    
    
    float4 albedo = UintToRGBA8(g_VoxelAlbedo[DTid]) / float4(255.0, 255, 255, 1);
    float3 normal = UnpackNormal(UintToRGBA8(g_VoxelNormal[DTid]).rgb / 255.0);
    
    /*if (albedo.a < EPSILON)
    {
        return;
    }*/

    float4 directLight = g_VoxelLit[DTid];

    // calculate indirect lighting - first bounce onto the voxel texture
    float3 position = float3(DTid) / voxelVolumeDimension;
    float4 indirectLighting = CalculateIndirectLighting(position, normal);
    indirectLighting *= albedo;
    
    float4 radiance = directLight + indirectLighting;
    radiance.a = directLight.a;

    g_VoxelFirstBounce[DTid] = radiance;
}