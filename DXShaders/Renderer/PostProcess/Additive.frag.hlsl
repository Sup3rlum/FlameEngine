
Texture2D gFrame : register(t0);
SamplerState FrameSampler : register(s0);

Texture2D gFrame2 : register(t1);
SamplerState FrameSampler2 : register(s1);


struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
    float4 Color = gFrame.Sample(FrameSampler, input.TexCoord);
    Color += gFrame2.Sample(FrameSampler2, input.TexCoord);
      
    return Color;
}