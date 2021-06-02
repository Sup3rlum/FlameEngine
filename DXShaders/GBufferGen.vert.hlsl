

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
	
	
    float3x3 normalMatrix = mul((float3x3) View, (float3x3)WorldInverseTranspose);
	
   /* float3 fNormal = mul(normalMatrix, input.Normal);
    float3 fTangent = mul(normalMatrix, input.Tangent);
    float3 fBitangent = mul(normalMatrix, input.Bitangent);
    output.TBN = float3x3(fTangent, fBitangent, fNormal);*/
	
    output.Normal = mul(normalMatrix, input.Normal);
	
    float4 worldPos = mul(World, float4(input.Position, 1.0f));
    float4 viewPos = mul(View, worldPos);
    output.Position = mul(Projection, viewPos);
	
	return output;
}