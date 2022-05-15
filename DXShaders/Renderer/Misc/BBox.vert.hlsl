

struct VSInput
{
    float3 Position : POSITION0;
    float3 Color : COLOR0;
};

struct PSInput
{
    float4 Position : SV_Position;
    float3 Color : COLOR0;
};

cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};



PSInput main( VSInput input )
{
    PSInput output;
    
    float4 viewPos = mul(View, float4(input.Position, 1.0f));
    output.Position = mul(Projection, viewPos);
    output.Color = input.Color;
    
    return output;
}