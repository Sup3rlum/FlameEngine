

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

Texture2D DepthMap : register(t4);
SamplerState DepthSampler : register(s4);



cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
    matrix InverseView;
    matrix InverseProjection;
};

cbuffer TransformationBuffer : register(b1)
{
    matrix World;
    matrix WorldInverseTranspose;
}

cbuffer MaterialPropertiesBuffer : register(b5)
{
    uint HasNormalMap;
    uint HasPOMEnabled;
    uint HasAOMap;
    uint HasEmissive;
    uint HasTransluscent;
}

float3 PackNormal(float3 n)
{
    return normalize(n) * 0.5f + 0.5f;
}


float GetDepth(float2 tCoord)
{
    return DepthMap.Sample(DepthSampler, tCoord).r;
}

float4 UnpackPosition(float2 tCoord)
{
    float z = GetDepth(tCoord);
    
    float x = tCoord.x * 2.0f - 1.0f;
    float y = (1 - tCoord.y) * 2 - 1.0f;
    
    float4 tRay = mul(InverseProjection, float4(x, y, z, 1.0f));
    return float4(tRay.xyz / tRay.w, 1.0f);
}

PSOutput main(PSInput input)
{
    PSOutput output;
    
    
    float4 fragPosViewSpace = UnpackPosition(input.Position.xy / float2(2560.0f, 1440.0f));
    float4 fragPosWorldSpace = mul(InverseView, float4(fragPosViewSpace.xyz, 1.0f));
    float4 fragPosDecalSpace = mul(transpose(WorldInverseTranspose), float4(fragPosWorldSpace.xyz, 1.0f));
    
    
    float2 texCoords = fragPosDecalSpace.xy * 0.5 + 0.5;
    float4 Color = DiffuseMap.Sample(DiffuseSampler, texCoords);
    float4 DF = DetailMap.Sample(DetailSampler, texCoords);

    clip(1.0 - abs(fragPosDecalSpace.xyz));
    clip(DF.g < 0.5f ? -1:1 );

    output.Albedo = float4(1,0,0, 1.0);
  
    output.Detail = float4(0, 0, 0, 0);
    output.Emission = float4(0, 0, 0, 0);
    output.Normal = float4(0.5, 0.5, 1.0, 0);
   
    
    return output;
}