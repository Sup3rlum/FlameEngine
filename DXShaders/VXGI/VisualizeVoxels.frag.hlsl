struct PSInput
{
    float4 Position : SV_Position;
    float4 Albedo : COLOR0;
};


float4 main(PSInput input) : SV_TARGET
{
	return input.Albedo;
}