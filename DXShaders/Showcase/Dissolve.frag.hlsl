

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
    float4 ViewPosition : VIEWPOS0;
};

struct PSOutput
{
    float4 Normal : SV_Target0;
    float4 Albedo : SV_Target1;
    float4 Detail : SV_Target2;
    float4 Emission : SV_Target3;
    
};

Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D DetailMap : register(t2);
Texture2D EmissiveMap : register(t3);

SamplerState DiffuseSampler : register(s0);
SamplerState NormalSampler : register(s1);
SamplerState DetailSampler : register(s2);
SamplerState EmissiveSampler : register(s3);

Texture2D DissolveMap : register(t4);
SamplerState DissolveSampler : register(s4);


cbuffer DissolveBuffer : register(b12)
{
    float4 dissolveTime;
}

/* 
    DetailMap:
    R - Height
    G - Roughness
    B - Metallic
    A - AO
*/

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

static float maxPOMDistance = 100;


float3 PackNormal(float3 n)
{
    return normalize(n) * 0.5f + 0.5f;
}


float3 CalculateNormalMap(PSInput input, float2 texCoords)
{
    float3 BumpMapNormal = NormalMap.Sample(NormalSampler, texCoords).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - float3(1.0f, 1.0f, 1.0f);
    float3 NewNormal;

    float3x3 TexSpace = float3x3(input.Tangent, input.Bitangent, input.Normal);
    
    NewNormal = mul(BumpMapNormal, TexSpace);
    NewNormal = normalize(NewNormal);
    return NewNormal;
}


#define heightScale 0.05f
#define parallaxBias 0.0
#define PARALLAX_RAYMARCHING_STEPS 10.0


float GetParallaxHeight(float2 uv)
{
    return 1.0 - DetailMap.Sample(DetailSampler, uv).r;
}


float2 ParallaxMapping(float2 texCoords, float3 viewDir, float fade)
{
    
    // number of depth layers
    const float minLayers = 0.0;
    const float maxLayers = 32.0;
    float numLayers = lerp(maxLayers, minLayers, max(dot(float3(0.0, 0.0, 1.0), viewDir), 0.0));
    numLayers = lerp(numLayers, minLayers, fade);

    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    float2 P = heightScale * viewDir.xy / (viewDir.z + parallaxBias);
    float2 deltaTexCoords = P / numLayers;
       
    
    float2 currentTexCoords = texCoords;
    float currentDepthMapValue = GetParallaxHeight(currentTexCoords); //HeightMap.Sample(HeightSampler, currentTexCoords).r;
  
    [loop]
    while (currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = GetParallaxHeight(currentTexCoords);
        currentLayerDepth += layerDepth;
    }
    
    float2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = GetParallaxHeight(prevTexCoords) - currentLayerDepth + layerDepth;
 
    float weight = afterDepth / (afterDepth - beforeDepth);
    float2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}


float3 GetTangentViewDir(PSInput input)
{
    
    float3x3 vTbn = float3x3(input.Tangent, input.Bitangent, input.Normal);
    float3 viewDir = normalize(input.ViewPosition.xyz);
    
    return mul(vTbn, viewDir);
}
float3 GetTangentLightDir(PSInput input)
{
    float3x3 vTbn = float3x3(input.Tangent, input.Bitangent, input.Normal);
    float3 LightDir = mul((float3x3) View, normalize(float3(0, -1, -1)));
    
    return mul(vTbn, LightDir);
}


PSOutput main(PSInput input)
{

    
    PSOutput output;
    
    float3 lightDir = GetTangentLightDir(input);
    float3 viewDir = GetTangentViewDir(input);
    float2 texCoords = input.TexCoord;
    float4 Detail = DetailMap.Sample(DetailSampler, texCoords);
    float4 Emissive = EmissiveMap.Sample(EmissiveSampler, texCoords);

    float Height = Detail.r;
    float Roughness = Detail.g;
    float Metallic = Detail.b;
    float AO = Detail.a;

    float dissolveMask = DissolveMap.Sample(DissolveSampler, texCoords);
    float dissolveThreshold = sin(dissolveTime.r) * 0.5f + 0.5f;
    
    if (dissolveMask < dissolveThreshold)
    {
        discard;
    }
    
    float3 dissolveColor = float3(0.0f, 0.2f, 1.0f);
    if (dissolveMask < dissolveThreshold + 0.03f)
    {
        Emissive.rgb += dissolveColor;
    }
    
     
    if (HasPOMEnabled)
    {
        float pomFadeFactor = clamp(length(input.ViewPosition.xyz) / maxPOMDistance, 0, 1);
        texCoords = ParallaxMapping(input.TexCoord, normalize(viewDir), pomFadeFactor);
    }
       
    if (HasNormalMap)
        output.Normal = float4(PackNormal(CalculateNormalMap(input, texCoords)), 1.0f);
    else
        output.Normal = float4(PackNormal(input.Normal), 1.0f);

    
    output.Albedo = DiffuseMap.Sample(DiffuseSampler, texCoords);
    if (HasAOMap) output.Albedo.rgb *= lerp(0.3, 1, AO);
    
    output.Detail = float4(1, 1, 1, 1);
    output.Detail.rgb = Detail.gba;
    output.Emission = Emissive;
   
    
    return output;
}