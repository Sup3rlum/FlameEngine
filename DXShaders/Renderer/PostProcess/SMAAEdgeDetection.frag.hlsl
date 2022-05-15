#include "SMAA.hlsl"

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float4 Offset[3] : TEXCOORD1;
};

Texture2D gInput : register(t0);

float4 main(PSInput input) : SV_TARGET
{
    return float4(SMAAColorEdgeDetectionPS(input.TexCoord, input.Offset, gInput), 0.0, 1.0);
}