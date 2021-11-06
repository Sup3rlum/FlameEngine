
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
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
	
    float3 ViewPos : VIEWPOS0;
};


cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};


cbuffer TransformationBuffer : register(b1)
{
    matrix World;
    matrix WorldInverseTranspose;
}



PSInput main(VSInput input)
{
    PSInput output;
    
    
    output.TexCoord = input.TexCoord;
	
	
    float3x3 normalMatrix = mul((float3x3) View, (float3x3) WorldInverseTranspose);
	
    output.Normal = mul(normalMatrix, normalize(input.Normal));
    output.Tangent = mul(normalMatrix, normalize(input.Tangent));
    output.Bitangent = mul(normalMatrix, normalize(input.Bitangent));

    
    float4 worldPos = mul(World, float4(input.Position, 1.0f));
    float4 viewPos = mul(View, worldPos);
    output.ViewPos = viewPos;
    output.Position = mul(Projection, viewPos);
    
    return output;
}