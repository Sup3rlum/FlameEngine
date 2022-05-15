

struct VSInput
{
    float3 Position : POSITION0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
    float2 TexCoord : TEXCOORD0;
    
    float4x4 InstanceWorld : INSTANCE_WORLD;
    float4 InstanceColor : INSTANCE_COLOR;
    
    uint InstanceID : SV_InstanceID;
};

struct PSInput
{
    float4 Position : SV_Position;
    float4 Color : COLOR0;
    float3 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
    
};



cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};


PSInput main(VSInput input)
{
    PSInput output;
    
    float3x3 normalMatrix = (float3x3)View;
	 
    float3 billBoardedPosition = mul(transpose(normalMatrix), input.Position);
    
    float4 worldPos = mul(input.InstanceWorld, float4(billBoardedPosition, 1));
    float4 viewPos = mul(View, worldPos);
    output.Position = mul(Projection, viewPos);
    output.Color = input.InstanceColor;
    output.Normal = mul(normalMatrix, input.Normal);
    output.TexCoord = input.TexCoord;
    
	return output;
}