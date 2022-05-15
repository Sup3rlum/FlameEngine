struct PSInput
{
    float4 Position : SV_Position;
    float4 Color : COLOR0;
    float3 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
};

/*
struct PSOutput
{
    float4 Normal : SV_Target0;
    float4 Albedo : SV_Target1;
    float4 RoughnessMetallic : SV_Target2;
    float4 Emission : SV_Target3;
};
*/


struct PSOutput
{
    float4 FragColor : SV_TARGET;
};


float3 PackNormal(float3 n)
{
    return (normalize(n) + 1.0f) / 2.0f;
}


Texture2D ParticleTexture : register(t0);
SamplerState ParticleSampler : register(s0);


PSOutput main(PSInput input)
{
    PSOutput output;
    

    /*output.Normal = float4(PackNormal(input.Normal), 1.0f);
    output.Albedo = ParticleTexture.Sample(ParticleSampler, input.TexCoord);
    output.RoughnessMetallic = float4(1, 0, 1, 1);
    output.Emission = 0;*/
    
    output.FragColor = ParticleTexture.Sample(ParticleSampler, input.TexCoord);
    
    return output;
}