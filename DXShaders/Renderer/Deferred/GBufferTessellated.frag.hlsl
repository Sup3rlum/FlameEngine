

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
    float4 ViewPosition : VIEWPOS0;
};

struct PSOutput
{
    float4 Normal : SV_Target0;
    float4 Albedo : SV_Target1;
    float4 Detail : SV_Target2;
    float4 Emission : SV_Target3;
    
};

Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D DetailMap : register(t2);
Texture2D EmissiveMap : register(t3);

SamplerState DiffuseSampler : register(s0);
SamplerState NormalSampler : register(s1);
SamplerState DetailSampler : register(s2);
SamplerState EmissiveSampler : register(s3);

/* 
    DetailMap:
    R - Height
    G - Roughness
    B - Metallic
    A - AO
*/

cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};


cbuffer MaterialPropertiesBuffer : register(b5)
{
    uint HasNormalMap;
    uint HasPOMEnabled;
    uint HasAOMap;
    uint HasEmissive;
    uint HasTransluscent;
}

static float maxPOMDistance = 100;



float3 PackNormal(float3 n)
{
    return normalize(n) * 0.5f + 0.5f;
}


float3 CalculateNormalMap(PSInput input, float2 texCoords)
{
    float3 BumpMapNormal = NormalMap.Sample(NormalSampler, texCoords).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - float3(1.0f, 1.0f, 1.0f);
    float3 NewNormal;

    float3x3 TexSpace = float3x3(input.Tangent, input.Bitangent, input.Normal);
    
    NewNormal = mul(BumpMapNormal, TexSpace);
    NewNormal = normalize(NewNormal);
    return NewNormal;
}



PSOutput main(PSInput input)
{
    PSOutput output;
    
    float2 texCoords = input.TexCoord;
    float4 Detail = DetailMap.Sample(DetailSampler, texCoords);
    float4 Emissive = EmissiveMap.Sample(EmissiveSampler, texCoords);

    float Height    = Detail.r;
    float Roughness = Detail.g;
    float Metallic  = Detail.b;
    float AO        = Detail.a;

       
    if (HasNormalMap)
        output.Normal = float4(PackNormal(CalculateNormalMap(input, texCoords)), 1.0f);
    else
        output.Normal = float4(PackNormal(input.Normal), 1.0f);

    
    output.Albedo = DiffuseMap.Sample(DiffuseSampler, texCoords);
    if (HasAOMap)
    {
        output.Albedo.rgb *= lerp(0.3, 1, AO);
    }
    
    output.Detail = float4(Detail.gba, 1.0f);   
    output.Emission = Emissive;  
    
    return output;
}