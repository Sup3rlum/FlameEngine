

#include "Utilities/SMAA.hlsl"

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float2 PixCoord : TEXCOORD1;
    float4 Offset[3] : TEXCOORD2;
};

Texture2D gEdges : register(t1);
Texture2D gAreaTex : register(t2);
Texture2D gSearchTex : register(t3);


float4 main(PSInput input) : SV_TARGET
{
    return SMAABlendingWeightCalculationPS(input.TexCoord, input.PixCoord, input.Offset, gEdges, gAreaTex, gSearchTex, float4(0, 0, 0, 0));

}