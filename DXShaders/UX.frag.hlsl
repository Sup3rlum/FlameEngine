
struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};

Texture2D UxTexture : register(t0);
SamplerState UxSampler : register(s0);

float4 main(PSInput input) : SV_Target0
{
    return UxTexture.Sample(UxSampler, input.TexCoord);
}