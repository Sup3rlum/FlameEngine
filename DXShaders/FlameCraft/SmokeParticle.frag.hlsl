struct PSInput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
    float2 TexCoord : TEXCOORD0;
    float Age : AGE0;
    float Life : LIFE0;
    float index : INDEX0;
};


struct PSOutput
{
    float4 FragColor : SV_TARGET;
};

Texture2D gDiffuse : register(t0);
SamplerState DiffuseSampler : register(s0);

Texture2D NormalMap : register(t1);
SamplerState NormalSampler : register(s1);


cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};


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
    
    float age = input.Age * 10.0f;
    float2 texCoord = float2(floor(fmod(age, 8)), floor(age / 8)) / 8.0f;  
    float3 normal = CalculateNormalMap(input, input.TexCoord / 8.0f + texCoord);
    
    float4 albedo = gDiffuse.Sample(DiffuseSampler, input.TexCoord / 8.0f + texCoord);
    
    if (albedo.a < 0.01f)
        discard;
    
    float lightDir = mul((float3x3) View, normalize(float3(1, 1, 1)));
    float albedoFactor = max(0.3f, dot(lightDir, normal));
    
    output.FragColor = albedo;
    output.FragColor.rgb *= albedoFactor;
    
    
    if (input.Age / input.Life > 0.6f)
        output.FragColor.a *= (input.Life - input.Age) / (0.4f * input.Life);
    
    return output;
}