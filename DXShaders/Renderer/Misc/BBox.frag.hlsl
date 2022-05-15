struct PSInput
{
    float4 Position : SV_Position;
    float3 Color : COLOR0;
};


float4 main(PSInput input) : SV_TARGET
{
	return float4(input.Color, 1.0f);
}