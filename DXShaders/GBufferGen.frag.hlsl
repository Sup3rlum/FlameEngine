

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL0;
};

struct PSOutput
{
    float4 Depth : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Albedo : SV_Target2;
    float4 Specular : SV_Target3;
    
};

Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D RoughnessMap : register(t2);

SamplerState DiffuseSampler : register(s0);
SamplerState NormalSampler : register(s1);
SamplerState RoughnessSampler : register(s2);


float3 PackNormal(float3 n)
{
    return (normalize(n) + 1.0f) / 2.0f;
}

/*float3 CalculateNormalMap(PSInput input)
{
    float3 BumpMapNormal = NormalMap.Sample(DiffuseSampler, input.TexCoord).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - float3(1.0f, 1.0f, 1.0f);
    float3 NewNormal;

    NewNormal = mul(input.TBN, BumpMapNormal);
    NewNormal = normalize(NewNormal);
    return NewNormal;
}
*/


PSOutput main(PSInput input)
{
    PSOutput output;
    
    float clipDepth = input.Position.z;
    float moment2 = clipDepth * clipDepth;
    
    output.Depth = float4(clipDepth, moment2, 0.0f, 1.0f);
    //output.Normal = float4(PackNormal(CalculateNormalMap(input)), 1.0f);
    output.Normal = float4(PackNormal(input.Normal), 1.0f);
    output.Albedo = DiffuseMap.Sample(DiffuseSampler, input.TexCoord);
    output.Specular = float4(1, 1, 1, 1);
    
    return output;
}