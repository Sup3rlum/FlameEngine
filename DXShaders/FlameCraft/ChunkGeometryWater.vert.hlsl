

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
    float3 WorldPos : WORLD_POS0;
    float3 ViewPos : VIEW_POS0;
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

struct WaveDesc
{
    float3 Displacement;
    float3 Normal;
};

WaveDesc GerstnerWave(float4 wave, float3 p)
{
    WaveDesc final;
    float steepness = wave.z;
    float wavelength = wave.w;
    float k = 2.0 * 3.1415 / wavelength;
    float c = sqrt(9.8 / k);
    float2 d = normalize(wave.xy);
    float f = k * (dot(d, p.xz) - c * waveTime.r);
    float a = steepness / k;

    float3 tangent = float3(
				1 - d.x * d.x * (steepness * sin(f)),
				d.x * (steepness * cos(f)),
				-d.x * d.y * (steepness * sin(f))
			);
    float3 binormal = float3(
				-d.x * d.y * (steepness * sin(f)),
				d.y * (steepness * cos(f)),
				1 - d.y * d.y * (steepness * sin(f))
			);
    
    final.Displacement = float3(
				d.x * (a * cos(f)),
				a * sin(f),
				d.y * (a * cos(f))
			);

    final.Normal = normalize(cross(binormal, tangent));
    
    return final;
}


PSInput main(VSInput input)
{
    PSInput output;
    output.TexCoord = input.TexCoord;
	
    float3x3 normalMatrix = mul((float3x3) View, (float3x3) WorldInverseTranspose);
    output.Normal = mul(normalMatrix, normalize(input.Normal));
    
    float3 pos = input.Position;
    
    WaveDesc wave1 = GerstnerWave(float4(1, 1, 0.1, 10), pos);
    WaveDesc wave2 = GerstnerWave(float4(1, 0.6, 0.1, 5.5f), pos);
    WaveDesc wave3 = GerstnerWave(float4(1, 1.3, 0.1, 3.5f), pos);
    WaveDesc wave4 = GerstnerWave(float4(1, 1, 0.08456, 7.345), pos);
    pos += wave1.Displacement;
    pos += wave2.Displacement;
    pos += wave3.Displacement;
    pos += wave4.Displacement;
    pos -= 0.2f;
    
    output.Normal = normalize(wave1.Normal + wave2.Normal + wave3.Normal + wave4.Normal);
    
    float4 worldPos = mul(World, float4(pos, 1.0f));
    float4 viewPos = mul(View, worldPos);
    output.Position = mul(Projection, viewPos);
    output.WorldPos = worldPos.xyz;
    output.ViewPos = viewPos.xyz;
	
    return output;
}