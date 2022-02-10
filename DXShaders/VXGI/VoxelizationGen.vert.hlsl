

#include "VXGIBuffers.hlsl"

struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
    float2 TexCoord : TEXCOORD0;
};


struct GSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
    float4 WorldPosition : WORLDPOS0;
	
};


cbuffer VoxelizeBuffer : register(b9)
{
    matrix Projection;
    matrix Projection2;
    matrix Projection3;
};


cbuffer TransformationBuffer : register(b1)
{
    matrix World;
    matrix WorldInverseTranspose;
}

GSInput main(VSInput input)
{
    GSInput output;
	
    output.TexCoord = input.TexCoord;
		
    float3x3 normalMatrix = (float3x3) WorldInverseTranspose;//mul((float3x3) View, (float3x3) WorldInverseTranspose);
	
    output.Normal = mul(normalMatrix, normalize(input.Normal));
    output.Tangent = mul(normalMatrix, normalize(input.Tangent));
    output.Bitangent = mul(normalMatrix, normalize(input.Bitangent));

	
    float4 worldPos = mul(World, float4(input.Position, 1.0f));
    output.WorldPosition = worldPos;
    
    
    float3 aabbCenter = (worldMaxPoint - worldMinPoint) * 0.5 + worldMinPoint;
    
    output.Position = mul(Projection, worldPos - float4(aabbCenter, 0));
	
    return output;
}
