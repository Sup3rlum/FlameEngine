struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};

static float offset[3] = { 0.0, 1.3846153846, 3.2307692308 };
static float weight[3] = { 0.2270270270, 0.3162162162, 0.0702702703 };

static float2 texSizeInv = float2(1.0f / 1280.0f, 1.0f / 720.0f);

Texture2D gInput : register(t0);
SamplerState Sampler : register(s0);


float4 main(PSInput input) : SV_TARGET
{
    float4 finalColor = gInput.Sample(Sampler, input.TexCoord) * weight[0];
    
    for (int i = 1; i < 3; i++)
    {
        finalColor += gInput.Sample(Sampler, input.TexCoord + float2(offset[i], 0.0) * texSizeInv) * weight[i];
        finalColor += gInput.Sample(Sampler, input.TexCoord - float2(offset[i], 0.0) * texSizeInv) * weight[i];
    }
    
    return finalColor;
}