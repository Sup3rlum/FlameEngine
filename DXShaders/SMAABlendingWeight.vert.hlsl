
#include "Utilities/SMAA.hlsl"

struct VSInput
{
    float3 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
};

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float2 PixCoord : TEXCOORD1;
    float4 Offset[3] : TEXCOORD2;
};


PSInput main( VSInput input ) 
{
    PSInput output;
    
    output.Position = float4(input.Position, 1.0f);
    output.TexCoord = input.TexCoord;
    
    SMAABlendingWeightCalculationVS(output.TexCoord, output.PixCoord, output.Offset);
    
    return output;
}