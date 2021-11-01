

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
    float3 vDirection : VDIRECTION0;
};


cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};





PSInput main(VSInput input)
{
    PSInput output;
    
    float4 viewPos = mul(View, float4(input.Position, 0));
    output.Position = mul(Projection, float4(viewPos.xyz, 1));
    output.vDirection = input.Position;
    
    return output;
}