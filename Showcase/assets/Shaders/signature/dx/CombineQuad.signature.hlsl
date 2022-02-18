struct VSInput
{
    float3 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
};



float4 main(VSInput input) : SV_POSITION
{
    return float4(input.Position, 1.0f);
}