struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
    float2 TexCoord : TEXCOORD0;
};

struct PSInput
{
    float4 Position : SV_Position;
    float4 WorldPos : WORLDPOS;
};

cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};

PSInput main(VSInput input)
{
    PSInput output;
	
    output.WorldPos = float4(input.Position, 1.0f);
    float4 viewPos = mul(View, output.WorldPos);
    output.Position = mul(Projection, viewPos);

    return output;
}