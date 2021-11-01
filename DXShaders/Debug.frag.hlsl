


Texture2DArray gShadowmap : register(t4);
SamplerState ShadowmapSampler : register(s4);


struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};


float4 main(PSInput input) : SV_TARGET
{
    
    
    float arrIndex = floor(input.TexCoord.x * 5);
    
    float x = input.TexCoord.x * 5.0;
    float y = input.TexCoord.y;
    
    return float4(gShadowmap.Sample(ShadowmapSampler, float3(x,y, arrIndex)).xy, 0, 1);

}