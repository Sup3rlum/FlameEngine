
struct PSInput
{
    float4 Position : SV_Position;
    float3 Color : COLOR0;
};

struct PSOutput
{
    float4 OutColor0 : SV_Target0;
    float4 OutColor1 : SV_Target1;
    float4 OutColor2 : SV_Target2;
    float4 OutColor3 : SV_Target3;
};


PSOutput main(PSInput input)
{
    PSOutput output;
    
    output.OutColor0 = float4(input.Color, 1.0f);
    output.OutColor1 = float4(input.Color, 1.0f);
    output.OutColor2 = float4(input.Color, 1.0f);
    output.OutColor3 = float4(input.Color, 1.0f);
    
    
    return output;
}