struct DSInput
{
    float3 Position : WORLDPOS;
    float3 Normal : NORMAL0;
    float3 TexCoord : TEXCOORD0;
};

struct PSInput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL0;
    float3 TexCoord : TEXCOORD0;
};

struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor;
	float InsideTessFactor			: SV_InsideTessFactor;
};


cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};

cbuffer TransformationBuffer : register(b1)
{
    matrix World;
    matrix WorldInverseTranspose;
}

cbuffer WaveBuffer : register(b12)
{
    float4 waveTime;
}


Texture2D WindDispl : register(t1);
SamplerState WindSampler : register(s1);


#define NUM_CONTROL_POINTS 3

[domain("tri")]
PSInput main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<DSInput, NUM_CONTROL_POINTS> patch)
{
	PSInput Output;

    float3 vertPos = 
	patch[0].Position * domain.x + 
	patch[1].Position * domain.y + 
	patch[2].Position * domain.z;
	
    float3 Normal = 
	patch[0].Normal * domain.x +
	patch[1].Normal * domain.y +
	patch[2].Normal * domain.z;

    float3 TexCoord =
	patch[0].TexCoord * domain.x +
	patch[1].TexCoord * domain.y +
	patch[2].TexCoord * domain.z;
	
	
    float windDispl = WindDispl.SampleLevel(WindSampler, fmod(float2((vertPos.x + waveTime.r), vertPos.z), 256.0f), 0.0f).r;
    //float windDispl = 0.5f * sin(waveTime.r * 5);
	
    vertPos.x += windDispl * (1.0 - TexCoord.y);
	
    float4 worldPos = mul(World, float4(vertPos, 1.0f));
    float4 viewPos = mul(View, worldPos);
    Output.Position = mul(Projection, viewPos);
    Output.Normal = Normal;
    Output.TexCoord = TexCoord;
	
	return Output;
}
