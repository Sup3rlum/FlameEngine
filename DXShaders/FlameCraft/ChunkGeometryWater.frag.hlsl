#include "../Utilities/Spaces.hlsl"
#include "../Utilities/CookTorrance.hlsl"

struct PSInput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL0;
    float3 TexCoord : TEXCOORD0;
    float3 WorldPos : WORLD_POS0;
    float3 ViewPos : VIEW_POS0;
};


Texture2D gDepth : register(t0);
Texture2D gNormal : register(t1);
Texture2D gAlbedo : register(t2);

Texture2DArray DiffuseMap : register(t3);

SamplerState DepthSampler : register(s0);
SamplerState NormalSampler : register(s1);
SamplerState AlbedoSampler : register(s2);

SamplerState DiffuseSampler : register(s1);


cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};


cbuffer MaterialPropertiesBuffer : register(b5)
{
    uint HasNormalMap;
    uint HasPOMEnabled;
    uint HasAOMap;
    uint HasEmissive;
    uint HasTransluscent;
}


cbuffer LightingPassBuffer : register(b4)
{
    matrix LView;
    matrix LProjection;
    matrix InverseView;
    matrix InverseProjection;
    
    float4 CameraPosition;
    float zNear;
    float zFar;
    float NumDLights;
    float NumPLights;
    float NumSLights;
}


float4 UnpackPosition(float depth, float2 tCoord)
{
    float z = depth;
    
    float x = tCoord.x * 2.0f - 1.0f;
    float y = (1 - tCoord.y) * 2 - 1.0f;
    
    float4 tRay = mul(InverseProjection, float4(x, y, z, 1.0f));
    return float4(tRay.xyz / tRay.w, 1.0f);
}

float ViewToProjDepth(float vsDepth)
{
    float4 res = mul(Projection, float4(0, 0, vsDepth, 1.0f));
    
    return res.z / res.w;

}


#define SSR_STEP_ITERATIONS 4
#define SSR_MAX_DISTANCE 100.0f
#define SSR_STEPS 16.0f

float2 SSR_ThirdSearch(float2 startFrag, float2 endFrag, float startDepth, float endDepth)
{

    float viewDepthDelta = (endDepth - startDepth) / SSR_STEPS;
    float2 fragDelta = (endFrag - startFrag) / SSR_STEPS;
    
    /* First 3rd of Ray */
    float4 sample = 0;
    float4 ray = 0;
    float4 comp = 0;
    float4 hit = 0;
    float4 distances = 0;
    float4 baseDistances = float4(1.0f, 2.0f, 3.0f, 4.0f);
    
    [unroll]
    for (int i = 0; i < SSR_STEP_ITERATIONS; i++)
    {
        distances = baseDistances + 4.0f * i;
    
        sample.r = gDepth.Sample(DepthSampler, startFrag + fragDelta * distances.r).r;
        sample.g = gDepth.Sample(DepthSampler, startFrag + fragDelta * distances.g).r;
        sample.b = gDepth.Sample(DepthSampler, startFrag + fragDelta * distances.b).r;
        sample.a = gDepth.Sample(DepthSampler, startFrag + fragDelta * distances.a).r;
          
        ray.r = ViewToProjDepth(startDepth + viewDepthDelta * distances.r);
        ray.g = ViewToProjDepth(startDepth + viewDepthDelta * distances.g);
        ray.b = ViewToProjDepth(startDepth + viewDepthDelta * distances.b);
        ray.a = ViewToProjDepth(startDepth + viewDepthDelta * distances.a);

        float4 compLo = ray > sample;
        float4 hitLo = compLo * distances;
        
        if (dot(hitLo, 1.0) > 0.0)
        {
            float4 test = (hitLo < 0.5f) * 30.0f + hitLo;
            float closest = min(min(test.x, test.y), min(test.z, test.w));
         
            return startFrag + closest * fragDelta;
        }
    }
  
    return endFrag;
}




float2 SSR_SecondSearch(float2 startFrag, float2 endFrag, float startDepth, float endDepth)
{

    float viewDepthDelta = (endDepth - startDepth) / SSR_STEPS;
    float2 fragDelta = (endFrag - startFrag) / SSR_STEPS;
    
    /* First 3rd of Ray */
    float4 sample = 0;
    float4 ray = 0;
    float4 comp = 0;
    float4 hit = 0;
    float4 distances = 0;
    float4 baseDistances = float4(1.0f, 2.0f, 3.0f, 4.0f);
    
    [unroll]
    for (int i = 0; i < SSR_STEP_ITERATIONS; i++)
    {
        distances = baseDistances + 4.0f * i;
    
        sample.r = gDepth.Sample(DepthSampler, startFrag + fragDelta * distances.r).r;
        sample.g = gDepth.Sample(DepthSampler, startFrag + fragDelta * distances.g).r;
        sample.b = gDepth.Sample(DepthSampler, startFrag + fragDelta * distances.b).r;
        sample.a = gDepth.Sample(DepthSampler, startFrag + fragDelta * distances.a).r;
          
        ray.r = ViewToProjDepth(startDepth + viewDepthDelta * distances.r);
        ray.g = ViewToProjDepth(startDepth + viewDepthDelta * distances.g);
        ray.b = ViewToProjDepth(startDepth + viewDepthDelta * distances.b);
        ray.a = ViewToProjDepth(startDepth + viewDepthDelta * distances.a);

        float4 compLo = ray > sample;
        float4 hitLo = compLo * distances;
        
        if (dot(hitLo, 1.0) > 0.0)
        {

            float4 test = (hitLo < 0.5f) * 30.0f + hitLo;
            float closest = min(min(test.x, test.y), min(test.z, test.w));
            float prevSample = closest - 1;
     
        
            return SSR_ThirdSearch(
        startFrag + prevSample * fragDelta,
        startFrag + closest * fragDelta,
        startDepth + prevSample * viewDepthDelta,
        startDepth + closest * viewDepthDelta);

        }
    }
  
    return endFrag.xy;
}


float2 SSR(float3 vsPos, float3 vsViewDir, float3 vsNormal)
{
    float3 rayDir = normalize(reflect(vsViewDir, vsNormal));
    
    float3 startPos = vsPos;
    float3 endPos = vsPos + SSR_MAX_DISTANCE * rayDir;
    
    float startDepth = startPos.z;
    float viewDepthDelta = (endPos.z - startPos.z) / SSR_STEPS;
    
    float4 startFrag = mul(Projection, float4(startPos, 1.0f));
    startFrag.xyz /= startFrag.w;
    startFrag.y *= -1.0f;
    startFrag.xy = startFrag.xy * 0.5 + 0.5;
    
    float4 endFrag = mul(Projection, float4(endPos, 1.0f));
    
    endFrag.xyz /= endFrag.w;
    endFrag.y *= -1.0f;
    endFrag.xy = endFrag.xy * 0.5 + 0.5;
    
    float2 fragDelta = (endFrag.xy - startFrag.xy) / SSR_STEPS;
    
    
    /* First 3rd of Ray */
    float4 sample = 0;
    float4 ray = 0;
    float4 comp = 0;
    float4 hit = 0;
    float4 distances = 0;
    float4 baseDistances = float4(1.0f, 2.0f, 3.0f, 4.0f);
    
    [unroll]
    for (int i = 0; i < SSR_STEP_ITERATIONS;i++)
    {
        distances = baseDistances + 4.0f * i;
    
        sample.r = gDepth.Sample(DepthSampler, startFrag.xy + fragDelta * distances.r).r;
        sample.g = gDepth.Sample(DepthSampler, startFrag.xy + fragDelta * distances.g).r;
        sample.b = gDepth.Sample(DepthSampler, startFrag.xy + fragDelta * distances.b).r;
        sample.a = gDepth.Sample(DepthSampler, startFrag.xy + fragDelta * distances.a).r;
          
        ray.r = ViewToProjDepth(startDepth + viewDepthDelta * distances.r);
        ray.g = ViewToProjDepth(startDepth + viewDepthDelta * distances.g);
        ray.b = ViewToProjDepth(startDepth + viewDepthDelta * distances.b);
        ray.a = ViewToProjDepth(startDepth + viewDepthDelta * distances.a);

        float4 compLo = ray > sample;
        float4 hitLo = compLo * distances;
        
        if (dot(hitLo, 1.0) > 0.0)
        {

            float4 test = (hitLo < 0.5f) * 30.0f + hitLo;
            float closest = min(min(test.x, test.y), min(test.z, test.w));
            float prevSample = closest - 1;
     
        
            return SSR_SecondSearch(
        startFrag.xy + prevSample * fragDelta,
        startFrag.xy + closest * fragDelta,
        startDepth + prevSample * viewDepthDelta,
        startDepth + closest * viewDepthDelta);

        }
    }
  
    return endFrag.xy;
}

float4 main(PSInput input) : SV_Target0
{
    float2 screenCoord = input.Position.xy / float2(2560.0, 1440.0);

    float3 LightDir = -normalize(float3(-1, -1, -1));
    
    float sampleDepth = gDepth.Sample(AlbedoSampler, screenCoord).r;
    float meshDepth = input.Position.z;
    
    float3 vsSamplePos = UnpackPosition(sampleDepth, screenCoord).xyz;
    float3 vsMeshPos = input.ViewPos;
    float vsDepthDifference = (vsSamplePos.z - vsMeshPos.z);
    
    
    float3 viewDir = normalize(CameraPosition.xyz - input.WorldPos);  
    float3 albedo = gAlbedo.Sample(DiffuseSampler, screenCoord).rgb;
    float3 waterColor = float3(0.0, 0.2, 0.45);
    
    float3 H = normalize(LightDir + viewDir);
    float NdotH = max(0.0, dot(input.Normal, H));
    float NdotL = max(0.0, dot(input.Normal, LightDir));
    float NdotV = dot(viewDir, input.Normal);
    
    float3 vsViewDir = mul((float3x3) View, viewDir);
    float3 vsNormal = mul((float3x3) View, input.Normal);
    
    float3 reflColor = 1;
    
    if (NdotV < 0.7)
    {
        float2 ssrCoords = SSR(input.ViewPos, -vsViewDir, vsNormal);
        reflColor = gAlbedo.Sample(AlbedoSampler, ssrCoords).rgb;
    }
    
    float3 refrDir = refract(-vsViewDir, vsNormal, 0.15f);
    float2 refrDelta = -(normalize(refrDir) * vsDepthDifference).xy * 2 / float2(2560.0f, 1440.0f);
    float3 refrColor = gAlbedo.Sample(DiffuseSampler, screenCoord + refrDelta).rgb;
    
    // Add light absorbtion based on depth
    float depthCoeff = exp(-vsDepthDifference * 0.01);
    refrColor = lerp(waterColor, refrColor, depthCoeff);
    
    float3 combinedColor = lerp(reflColor, refrColor, NdotV);
    
    float3 diffuse = combinedColor * NdotL;
    float specular = pow(NdotH, 200.0f);
    
    float3 finalColor = diffuse + specular;
    finalColor = saturate(finalColor);
    
    
    
    
    
    float FogStart = 150.0f;
    float FogEnd = 350.0f;
    float3 FogColor = float3(0.5f, 0.6f, 0.7f);
    float3 SunColor = float3(1.0f, 0.9f, 0.7f);
    float FogThickness = 0.33f;
    
    float fogDepth = (length(vsMeshPos) - FogStart) / (FogEnd - FogStart);
    float VdotL = max(dot(viewDir, LightDir), 0.0);
    float3 fogColor = lerp(FogColor, SunColor, pow(VdotL, 8.0f));
    
    if (length(vsMeshPos) > FogStart)
    {
        float fogFactor = exp(-fogDepth * fogDepth * FogThickness);
        finalColor = lerp(fogColor, finalColor, fogFactor);
    }

    
    float4 output = float4(finalColor, 1.0);
    return output;
}