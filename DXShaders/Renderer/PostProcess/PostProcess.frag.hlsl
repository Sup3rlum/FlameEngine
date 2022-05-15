#include "../../Utilities/ACES.hlsl"


Texture2D gFrame : register(t0);
SamplerState FrameSampler : register(s0);


struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};


float4 main(PSInput input) : SV_TARGET
{
    float4 Color = gFrame.Sample(FrameSampler, input.TexCoord);
    
    Color.rgb = ACESFilm(Color.rgb);
    //Color.rgb = ExposureTonemap(Color.rgb, 1.2f);
    //Color.rgb = LinearToSRGB(Color.rgb);
    
    return Color;
}