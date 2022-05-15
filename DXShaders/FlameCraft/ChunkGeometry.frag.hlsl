

struct PSInput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL0;
    float3 TexCoord : TEXCOORD0;
};

struct PSOutput
{
    float4 Normal : SV_Target0;
    float4 Albedo : SV_Target1;
    float4 Detail : SV_Target2;
    float4 Emission : SV_Target3;
    
};

Texture2DArray DiffuseMap : register(t0);
SamplerState DiffuseSampler : register(s0);


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


PSOutput main(PSInput input)
{
    PSOutput output;

    output.Albedo = DiffuseMap.Sample(DiffuseSampler, input.TexCoord);
    
    if (output.Albedo.a < 0.1f)
        discard;
    
    
    output.Normal = float4(PackNormal(input.Normal), 1.0f);
    output.Detail = float4(1.0f, 0.0f, 1.0f, 1.0f);
    output.Emission = 0;   
    output.Albedo.a = 1.0f;
    
    return output;
}