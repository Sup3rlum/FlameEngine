
struct DSInput
{
    float4 Position : WORLDPOS0;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
    float4 ViewPosition : VIEWPOS0;
};

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
    float4 ViewPosition : VIEWPOS0;
	
};


struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor;
	float InsideTessFactor			: SV_InsideTessFactor;
};

Texture2D DetailMap : register(t2);
SamplerState DetailSampler : register(s2);


cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};


#define NUM_CONTROL_POINTS 3

[domain("tri")]
PSInput main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<DSInput, NUM_CONTROL_POINTS> patch)
{
	PSInput Output;

    float4 Position =
    patch[0].Position * domain.x +
    patch[1].Position * domain.y +
    patch[2].Position * domain.z;
    
    
    float4 ViewPosition =
    patch[0].ViewPosition * domain.x +
    patch[1].ViewPosition * domain.y +
    patch[2].ViewPosition * domain.z;
    
    // Normals are in view-space
    float3 Normal =
    patch[0].Normal * domain.x +
    patch[1].Normal * domain.y +
    patch[2].Normal * domain.z;
    
    float3 Tangent =
    patch[0].Tangent * domain.x +
    patch[1].Tangent * domain.y +
    patch[2].Tangent * domain.z;
    
    float3 Bitangent =
    patch[0].Bitangent * domain.x +
    patch[1].Bitangent * domain.y +
    patch[2].Bitangent * domain.z;
    
    
    
    float2 TexCoord =
    patch[0].TexCoord * domain.x +
    patch[1].TexCoord * domain.y +
    patch[2].TexCoord * domain.z;

    float4 detail = DetailMap.SampleLevel(DetailSampler, TexCoord, 0);
    float height = detail.r;
    
    // Add to viewPosition since normals are in view-space
    ViewPosition.xyz += Normal * height * 0.4;
     
    Output.Position = mul(Projection, ViewPosition);
    Output.ViewPosition = ViewPosition;
    Output.Normal = Normal;
    Output.Tangent = Tangent;
    Output.Bitangent = Bitangent;
    Output.TexCoord = TexCoord;
    
    
	return Output;
}
