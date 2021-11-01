

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
    float4 RoughnessMetallic : SV_Target2;
    float4 Emission : SV_Target3;
    
};

Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D HeightMap : register(t2);
Texture2D RoughnessMap : register(t3);
Texture2D MetallicMap : register(t4);
Texture2D AOMap : register(t5);

SamplerState DiffuseSampler : register(s0);
SamplerState NormalSampler : register(s1);
SamplerState HeightSampler : register(s2);
SamplerState RoughnessSampler : register(s3);
SamplerState MetallicSampler : register(s4);
SamplerState AOSampler : register(s5);


cbuffer EmissionPropertiesBuffer : register(b5)
{
    float4 Emission;
    float4 CameraPos;
}

cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};


/*
cbuffer CombineBuffer : register(b4)
{
    matrix InverseView;
    matrix InverseProjection;
    float4 DirLightDirection;
    float4 DirLightInfo;
    
    float AmbienceFactor;
    float Exposure;
    float Padding0;
    float Padding1;
    
}*/


float3 PackNormal(float3 n)
{
    return normalize(n) * 0.5f  + 0.5f;
}


float3 CalculateNormalMap(PSInput input, float2 texCoords )
{
    float3 BumpMapNormal = NormalMap.Sample(DiffuseSampler, texCoords).xyz;
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

/*
float2 ParallaxRaymarch(float texCoords, float3 viewDir)
{
    
}*/


float GetParallaxHeight(float2 uv)
{
    return 1.0 - HeightMap.Sample(HeightSampler, uv).r;
}




float2 ParallaxMapping(float2 texCoords, float3 viewDir)
{

    
    // number of depth layers
    const float minLayers = 0.0;
    const float maxLayers = 32.0;
    float numLayers = lerp(maxLayers, minLayers, max(dot(float3(0.0, 0.0, 1.0), viewDir), 0.0));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    float2 P = heightScale * viewDir.xy / (viewDir.z + parallaxBias);
    float2 deltaTexCoords = P / numLayers;
       
    
    float2 currentTexCoords = texCoords;
    float currentDepthMapValue = GetParallaxHeight(currentTexCoords);//HeightMap.Sample(HeightSampler, currentTexCoords).r;
  
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

float ShadowCalc(float2 texCoord, float3 lightDir)
{
    if (lightDir.z >= 0.0)
        return 0.0;

    float minLayers = 0;
    float maxLayers = 32;
    float numLayers = lerp(maxLayers, minLayers, abs(dot(float3(0.0, 0.0, 1.0), lightDir)));

    float2 currentTexCoords = texCoord;
    float currentDepthMapValue = GetParallaxHeight(currentTexCoords);
    float currentLayerDepth = currentDepthMapValue;

    float layerDepth = 1.0 / numLayers;
    float2 P = lightDir.xy / lightDir.z * heightScale;
    float2 deltaTexCoords = P / numLayers;

    [loop]
    while (currentLayerDepth <= currentDepthMapValue && currentLayerDepth > 0.0)
    {
        currentTexCoords += deltaTexCoords;
        currentDepthMapValue = GetParallaxHeight(currentTexCoords).r;
        currentLayerDepth -= layerDepth;
    }

    float r = currentLayerDepth > currentDepthMapValue ? 0.0 : 1.0;
    return r;
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
    float2 pomTexCoords = ParallaxMapping(input.TexCoord, normalize(viewDir));
    float shadow = ShadowCalc(pomTexCoords, normalize(lightDir));
    
    output.Normal = float4(PackNormal(CalculateNormalMap(input, pomTexCoords)), 1.0f);
    output.Albedo = DiffuseMap.Sample(DiffuseSampler, pomTexCoords);
    
    output.Albedo.rgb *= lerp(0.3, 1, AOMap.Sample(AOSampler, pomTexCoords).r);
    
    
    output.RoughnessMetallic = float4(1, 1, 1, 1);
    output.RoughnessMetallic.r = RoughnessMap.Sample(RoughnessSampler, pomTexCoords);
    output.RoughnessMetallic.g = MetallicMap.Sample(MetallicSampler, pomTexCoords);
    
    output.Emission = Emission;
    
    return output;
}