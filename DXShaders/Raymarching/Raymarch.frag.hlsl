
struct PSInput
{
	float4 Position : SV_Position;
	float2 TexCoord : TEXCOORD0;
};

struct PSOutput
{
	float4 Normal : SV_Target0;
	float4 Albedo : SV_Target1;
	float4 Detail : SV_Target2;
	float4 Emission : SV_Target3;
    
};



Texture3D SDFTexture : register(t0);
SamplerState SDFSampler : register(s0);


cbuffer CameraMatrixBuffer : register(b0)
{
	matrix View;
	matrix Projection;
	matrix InverseView;
	matrix InverseProjection;
}


/*-------------*/


#define F0 0.04
#define PI 3.14159265f
#define HALF_PI 1.57079f
#define MAX_STEPS 128
#define EPSILON 0.01
#define SURFACE_BIAS 0.5
#define CUBE_SIZE 64
#define SCALE 0.1f

bool RayBoxIntersectPoint(
    float3 ro, float3 rd,
    float3 boxMin, float3 boxMax,
    out float3 hitPoint)
{
	float3 invDir = 1.0 / rd;

	float3 t0s = (boxMin - ro) * invDir;
	float3 t1s = (boxMax - ro) * invDir;

	float3 tsmaller = min(t0s, t1s);
	float3 tbigger = max(t0s, t1s);

	float tmin = max(max(tsmaller.x, tsmaller.y), tsmaller.z);
	float tmax = min(min(tbigger.x, tbigger.y), tbigger.z);

	bool hit = (tmax >= max(tmin, 0.0));

	if (hit)
	{
		float t = (tmin > 0.0) ? tmin : tmax;
		hitPoint = ro + rd * t;
	}
	else
	{
		hitPoint = float3(0, 0, 0);
	}

	return hit;
}

float SampleSDF(float3 p)
{
    float4 sdfDist = SDFTexture.Sample(SDFSampler, p / (CUBE_SIZE * SCALE));
	return sdfDist.r;
}

void MakeRay(float2 uvNDC, out float3 ro, out float3 rd)
{
	float4x4 invViewProj = mul(InverseView, InverseProjection);
	
    // uvNDC should be in range [-1,1] (x,y) where y up
    // create clip pos at near plane z = 0 (or -1 depending on API). We'll use z = 0 and w = 1
	float4 clipNear = float4(uvNDC.x, uvNDC.y, 0.0f, 1.0f);
	float4 worldNear = mul(invViewProj, clipNear);
	worldNear /= worldNear.w;

    // and a far point
	float4 clipFar = float4(uvNDC.x, uvNDC.y, 1.0f, 1.0f);
	float4 worldFar = mul(invViewProj, clipFar);
	worldFar /= worldFar.w;

	ro = mul(InverseView, float4(0, 0, 0, 1));
	rd = normalize(worldFar.xyz - worldNear.xyz);
}

float4 RayMarch(float2 uv, out float3 hitPos)
{
	uv = uv * 2.0 - 1.0;
	uv.y *= -1;

	float3 ro, rd;
	MakeRay(uv, ro, rd);
	
	float t = 0.0;
	float3 pos = ro;
	bool hit = false;
	float dist = 1e20;
	int steps = min(MAX_STEPS, 256);

    // Ray-march loop
	for (int i = 0; i < MAX_STEPS; ++i)
	{
		pos = ro + rd * t;
        // compute exact min distance to union-of-spheres
		float dScene = SampleSDF(pos);

        // If inside (dScene < EPSILON) we hit
		if (dScene < EPSILON)
		{
			hit = true;
			dist = dScene;
			hitPos = pos;
			break;
		}

        // advance ray: use conservative factor (surfaceBias) to avoid overshoot.
        // using a safety multiplier < 1 is more conservative.
		float step = max(0.001, dScene * SURFACE_BIAS);
		t += step;

		if (t >= CUBE_SIZE)
			break;
	}

	if (!hit)
	{
        // background color (maybe sky)
		return float4(0, 0, 0, 0);
	}
	
	return float4(0.2, 0.2, 0.2, 1);
}

float3 calcNormal(float3 p)
{
	float e = 1.0; // finite difference step
	float dx = SampleSDF(p + float3(e, 0, 0)) - SampleSDF(p - float3(e, 0, 0));
	float dy = SampleSDF(p + float3(0, e, 0)) - SampleSDF(p - float3(0, e, 0));
	float dz = SampleSDF(p + float3(0, 0, e)) - SampleSDF(p - float3(0, 0, e));
	return mul(View, float4(normalize(float3(dx, dy, dz)), 0)).xyz;
}

/* ------------------------------------------------------------------------------------ */

PSOutput main(PSInput input) 
{
	PSOutput output;

	float3 hitPos;
	float4 color = RayMarch(input.TexCoord, hitPos);
	
	if (color.a == 0)
		clip(-1);
	
	output.Albedo = color;
	output.Normal = float4(calcNormal(hitPos) * 0.5 + 0.5, 1);
	output.Detail = float4(0, 0, 0, 0);
	output.Emission = float4(0, 0, 0, 0);
	
	return output;

}
