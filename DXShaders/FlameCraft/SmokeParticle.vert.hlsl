

struct VSInput
{
    float3 Position : POSITION0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
    float2 TexCoord : TEXCOORD0;
    
    float4 InstancePosition : INSTANCE_POS;
    float4 Velocity : INSTANCE_VEL;
    float4 Data : INSTANCE_DATA;
    
    uint InstanceID : SV_InstanceID;
};

struct PSInput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
    float2 TexCoord : TEXCOORD0;
    float Age : AGE0;
    float Life : LIFE0;
    float Index : INDEX0;
    
};

cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};


PSInput main(VSInput input)
{
    PSInput output;
    

    float4 viewPos = mul(View, float4(input.InstancePosition.xyz, 1.0f));
    viewPos.xyz += input.Position * clamp(pow(input.Data.y, 0.5), 0.0, 1.0f);
    
    output.Position = mul(Projection, viewPos);
    
    output.Life = input.Data.x;
    output.Age = input.Data.y; 
    output.Normal = input.Normal;
    output.Tangent = input.Tangent;
    output.Bitangent = input.Bitangent;
    output.TexCoord = input.TexCoord;
    output.TexCoord.y = 1.0f - output.TexCoord.y;
    output.Index = input.InstanceID;
    
    return output;
}