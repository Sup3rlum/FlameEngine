#include "SMAA.hlsl"

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float4 Offset : TEXCOORD1;
};


Texture2D gBlendTex : register(t1);
Texture2D gColorTex : register(t0);


float4 main(PSInput input) : SV_TARGET
{
    return SMAANeighborhoodBlendingPS(input.TexCoord, input.Offset, gColorTex, gBlendTex);
}