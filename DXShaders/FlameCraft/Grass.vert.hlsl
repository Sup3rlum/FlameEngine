
struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL0;
    float3 TexCoord : TEXCOORD0;
};

struct PSInput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL0;
    float3 TexCoord : TEXCOORD0;
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


PSInput main(VSInput input)
{
    PSInput output;
    
    float3 vertPos = input.Position;
    
    float windScale = 0.6f;
    float windSpeed = 0.5f;
    float2 windDirection = float2(1, 1);
    float2 displCoords = fmod(float2((vertPos.x + waveTime.r * windSpeed), vertPos.z), 256.0f);
    
    
    float windDispl = WindDispl.SampleLevel(WindSampler, displCoords, 0.0f).r;
    windDispl -= 0.5f;
    
    vertPos.xz += windDispl * (1.0 - input.TexCoord.y) * normalize(windDirection);
    
	
    float3x3 normalMatrix = mul((float3x3) View, (float3x3) WorldInverseTranspose);
    output.Normal = mul(normalMatrix, normalize(input.Normal));
    output.TexCoord = input.TexCoord;
    
    float4 worldPos = mul(World, float4(vertPos, 1.0f));
    float4 viewPos = mul(View, worldPos);
    output.Position = mul(Projection, viewPos);
	
    return output;
}