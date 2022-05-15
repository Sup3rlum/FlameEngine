

#include "../../Utilities/Packing.hlsl"
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


RWTexture3D<uint> g_VoxelAlbedo : register(u1);
RWTexture3D<uint> g_VoxelNormal : register(u2);
RWTexture3D<uint> g_VoxelEmissive : register(u3);

Texture2D DiffuseMap : register(t0);
SamplerState DiffuseSampler : register(s0);


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


bool IsInCube(float3 pos)
{
    bool3 sides = pos <= 1.0 && pos >= 0.0;
    return all(sides);
}


void StoreAvg(RWTexture3D<uint> Grid, int3 voxelPos, float4 Value)
{   
    Value.rgb *= 255.0; // optimize following calculations
    uint newVal = RGBA8ToUint(Value);
    uint prevStoredVal = 0;
    uint curStoredVal = 0;
    uint numIterations = 0;
    uint originalValue = 0;
    
    InterlockedCompareExchange(Grid[voxelPos], prevStoredVal, newVal, curStoredVal);
    
    [loop]
    while ((curStoredVal = originalValue)
            != prevStoredVal
            && numIterations < 255)
    {
        InterlockedCompareExchange(Grid[voxelPos], prevStoredVal, newVal, originalValue);
        
        prevStoredVal = curStoredVal;
        float4 rval = UintToRGBA8(curStoredVal);
        rval.rgb = (rval.rgb * rval.a); // Denormalize
        float4 curValF = rval + Value; // Add
        curValF.rgb /= curValF.a; // Renormalize
        newVal = RGBA8ToUint(curValF);

        ++numIterations;
    }
    
    
    
}

float3 PackNormal(float3 n)
{
    return normalize(n) * 0.5f + 0.5f;
}


float4 main(PSInput input) : SV_TARGET
{        
    float3 texPos = WorldToVoxel(input.WorldPosition.xyz);
    int3 voxelPos = int3(texPos * voxelVolumeDimension);
   
    if (!IsInCube(texPos))
        discard;
    
    
    float4 albedo = float4(DiffuseMap.Sample(DiffuseSampler, input.TexCoord).rgb, 1.0);
    StoreAvg(g_VoxelAlbedo, voxelPos, albedo);   
    StoreAvg(g_VoxelNormal, voxelPos, float4(PackNormal(input.Normal), 1));
    StoreAvg(g_VoxelEmissive, voxelPos, float4(float3(voxelPos) / 256.0, 1.0));
    
    
    albedo.rgb *= max(0, dot(input.Normal, normalize(float3(1, 1, 1))));
    
    return albedo;
}