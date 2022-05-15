
cbuffer CameraMatrices : register(b0)
{
    matrix View;
    matrix Projection;
}

cbuffer TransformBuffer : register(b1)
{
    matrix World;
    matrix WorldInverseTranspose;
}

struct VSInput
{
    float3 Position : POSITION0;
};

struct PSInput
{
    float4 Position : SV_Position;
};

PSInput main(VSInput input)
{
    PSInput output;
    
    float4 WorldPos = mul(World, float4(input.Position, 1.0f));
    float4 ViewPos = mul(View, WorldPos);
    output.Position = mul(Projection, ViewPos);
   
    return output;
}