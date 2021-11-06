
#include "Utilities/CookTorrance.hlsl"

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};




Texture2D TransluscentMap : register(t8);
SamplerState TransluscentSampler : register(s8);



float4 main(PSInput input) : SV_TARGET
{
    float4 finalColor = TransluscentMap.Sample(TransluscentSampler, input.TexCoord);
    
    return finalColor;
}