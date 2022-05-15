
struct HSInput
{
    float3 Position : WORLDPOS;
    float3 Normal : NORMAL0;
    float3 TexCoord : TEXCOORD0;
};

struct DSInput
{
    float3 Position : WORLDPOS;
    float3 Normal : NORMAL0;
    float3 TexCoord : TEXCOORD0;
};

struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor;
	float InsideTessFactor			: SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 3

HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<HSInput, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	Output.EdgeTessFactor[0] = 
		Output.EdgeTessFactor[1] = 
		Output.EdgeTessFactor[2] = 
		Output.InsideTessFactor = 3;

	return Output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
DSInput main( 
	InputPatch<HSInput, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	DSInput Output;

	Output.Position = ip[i].Position;
    Output.TexCoord = ip[i].TexCoord;
    Output.Normal = ip[i].Normal;

	return Output;
}
