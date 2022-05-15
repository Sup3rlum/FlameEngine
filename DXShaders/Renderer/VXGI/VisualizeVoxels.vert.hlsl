
#include "VXGIBuffers.hlsl"

struct VSInput
{
    float3 Pos : POSITION;
};


struct GSInput
{
    float4 Position : SV_Position;
    float4 Albedo : COLOR0;
};


Texture3D VoxelMap : register(t0);


cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
    matrix InverseView;
    float4 CameraPosition;
};


GSInput main( VSInput input ) 
{
    GSInput output;
   

    int3 voxelPos = int3(input.Pos);
    
    output.Albedo = VoxelMap.Load(int4(voxelPos, 0));
    output.Position = float4(VoxelToWorld(input.Pos), 1.0f);
    
    
	return output;
}