
#include "../../Utilities/CookTorrance.hlsl"
#include "../../Utilities/Spaces.hlsl"
#include "../../Utilities/LightData.hlsl"

#define SM_CASCADES 5

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};


Texture2D gDepth : register(t0);
Texture2D gNormal : register(t1);
Texture2D gAlbedo : register(t2);
Texture2D gRoughnessMetallic : register(t3);
Texture2D gEmissive : register(t4);
Texture2D gHbao : register(t5);
Texture2DArray gShadowmap : register(t6);

Texture2D gBRDF_LUT : register(t7);
TextureCube EnvMap : register(t8);
TextureCube IrrMap: register(t9);


SamplerState DepthSampler : register(s0);
SamplerState NormalSampler : register(s1);
SamplerState AlbedoSampler : register(s2);
SamplerState RMSampler : register(s3);
SamplerState EmissiveSampler : register(s4);
SamplerState HbaoSampler : register(s5);
SamplerState ShadowmapSampler : register(s6);
SamplerState BRDFSampler : register(s7);
SamplerState EnvSampler : register(s8);
SamplerState IrradianceSampler : register(s9);


cbuffer LightingPassBuffer : register(b4)
{
    matrix View;
    matrix Projection;
    matrix InverseView;
    matrix InverseProjection;
    
    float4 CameraPosition; 
    float zNear;
    float zFar;
    float NumDLights;
    float NumPLights;
    float NumSLights;
}

cbuffer CascadeBuffer : register(b2)
{
    CascadeInfo SunCascades[SM_CASCADES];
}

#define MAX_LIGHTS 8

cbuffer DirectionalLightData : register(b6)
{
    DirectionalLight    DirectionalLights[MAX_LIGHTS];
}
cbuffer PointLightData : register(b7)
{
    PointLight          PointLights[MAX_LIGHTS];
}
cbuffer SpotLightData : register(b8)
{
    SpotLight           SpotLights[MAX_LIGHTS];
}

/*-------------*/


#define F0 0.04
#define PI 3.14159265f
#define HALF_PI 1.57079f


/* ------------------------------------------------------------------------------------ */

float3 UnpackNormal(float2 tCoord)
{
    return normalize(gNormal.Sample(NormalSampler, tCoord).rgb * 2 - 1.0f);
}

float GetDepth(float2 tCoord)
{
    return gDepth.Sample(DepthSampler, tCoord).r;
}

float GetLinearDepth(float2 tCoord)
{
    return LinearizeDepth(gDepth.Sample(DepthSampler, tCoord).r, zNear, zFar);
}


float GetLinearDepthTexelRaw(int2 pixel)
{
    return LinearizeDepth(gDepth.Load(int3(pixel, 0)).r, zNear, zFar);
}


float4 UnpackPosition(float2 tCoord)
{ 
    float z = GetDepth(tCoord);
    
    float x = tCoord.x * 2.0f - 1.0f;
    float y = (1 - tCoord.y) * 2 - 1.0f;
    
    float4 tRay = mul(InverseProjection, float4(x, y, z, 1.0f));
    return float4(tRay.xyz / tRay.w, 1.0f);
}

float VarianceShadow(float2 moments, float compare)
{
    
    float fAvgZ = moments.x;
    float fAvgZ2 = moments.y;
    
    if (compare <= fAvgZ)
    {
        return 1;
    }
    else
    {
        float variance = fAvgZ2 - (fAvgZ * fAvgZ);
        variance = max(variance, 0.00001f);

        float d = compare - fAvgZ;
        float pMax = smoothstep(0.0, 1.0, variance / (variance + d * d));

        return pow(pMax, 4.0);
    }
}

float ShadowCalculation(float4 fragPosLightSpace, float cascadeIndex)
{
    float4 shadowMapCoord = fragPosLightSpace / fragPosLightSpace.w;
    shadowMapCoord.y = -shadowMapCoord.y;
    float2 texCoords = shadowMapCoord.xy * 0.5 + 0.5;

    float depthCompare = shadowMapCoord.z;
    float2 moments = gShadowmap.Sample(ShadowmapSampler, float3(texCoords, (float)cascadeIndex)).xy;

    return VarianceShadow(moments, depthCompare);
}




/*******************************************************/



float3 CalcPointLight(PointLight pLight, PBRData pbr)
{  
    float3 LightPosViewSpace = mul(View, float4(pLight.Position.xyz, 1.0)).xyz;
    pbr.LightDir = normalize(LightPosViewSpace - pbr.FragPos);
    
    // Calculate point light attenuation
    float distance      = length(LightPosViewSpace - pbr.FragPos);
    float attDenom      = (distance / max(pLight.Radius, 0.001) + 1.0);
    float attenuation   = pLight.Radiance.a / (attDenom * attDenom);
    float3 radianceIn   = pLight.Radiance.rgb * attenuation;
    
    // Multiply radiance coeff by BRDF factor
    float3 radianceOut = BRDF(pbr) * radianceIn;
    return radianceOut;
}

float3 CalcSpotLight(SpotLight sLight, PBRData pbr)
{
    float3 vsLightPosition = mul(View, float4(sLight.Position.xyz, 1.0)).xyz;
    float3 vsLightDirection = mul((float3x3)View, sLight.Direction.xyz);
    pbr.LightDir = normalize(vsLightPosition - pbr.FragPos);
    
    // Calculate spot light aperture incidence
    float DdotL = dot(vsLightDirection, -pbr.LightDir);
    float apertureFactor = DdotL >= sLight.ApertureSize;
    
    // Calculate point light attenuation
    float distance = length(vsLightPosition - pbr.FragPos);
    float attDenom = (distance / max(sLight.Radius, 0.001) + 1.0);
    float attenuation = sLight.Radiance.a / (attDenom * attDenom);
    float3 radianceIn = sLight.Radiance.rgb * attenuation * apertureFactor;
   
    
    // Multiply radiance coeff by BRDF factor
    float3 radianceOut = BRDF(pbr) * radianceIn;
    return radianceOut;
}



float3 CalcDirectionalLight(DirectionalLight dLight, PBRData pbr)
{
    int cascadeIdx = 0;
    float fDepth = pbr.FragPos.z;
    
    for (int i = 0; i < SM_CASCADES; ++i)
    {
        float cascadeViewDepthFar = SunCascades[i].MaxDepth.x;
        
        if (fDepth < cascadeViewDepthFar)
        {
            cascadeIdx = i;
            break;
        }
    }
    
    float4 wsPosition = mul(InverseView, float4(pbr.FragPos, 1.0));
    float4 lsVPosition = mul(SunCascades[cascadeIdx].LView, wsPosition);
    float4 lsPosition = mul(SunCascades[cascadeIdx].LProjection, lsVPosition);
    float Shadow = ShadowCalculation(lsPosition, cascadeIdx);

    pbr.LightDir = -normalize(mul((float3x3) View, dLight.Direction.xyz));  
    float3 radianceIn = dLight.Radiance.rgb * dLight.Radiance.a * Shadow;

    float3 radianceOut = BRDF(pbr) * radianceIn;       
    return radianceOut;
}

float3 CalcAmbientLight(PBRData pbr)
{
    float3 wsNormal = mul(InverseView, float4(pbr.Normal, 0.0)).rgb;
    float3 wsViewDir = mul(InverseView, float4(pbr.ViewDir, 0.0)).rgb;
    float3 Refl = reflect(-wsViewDir, wsNormal);
    float NdotV = max(dot(wsNormal, wsViewDir), 0.0);

    float3 F = fresnelSchlickRoughness(NdotV, pbr.Fresnel0, pbr.Roughness);
    float3 kD = (1.0 - F) * (1.0 - pbr.Metallic);

    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.

    float3 irradiance = IrrMap.Sample(IrradianceSampler, wsNormal).rgb;
    float3 diffuse = irradiance * pbr.Albedo * kD * pbr.AO;


    float4 hdrColor = EnvMap.SampleLevel(EnvSampler, Refl, pbr.Roughness * 8.0f);
    float3 prefilteredColor = pow(hdrColor.rgb * hdrColor.a, 1.5f) * (1 - pbr.Roughness);

    float2 brdf = gBRDF_LUT.Sample(BRDFSampler, float2(NdotV, 1.0f - sqrt(pbr.Roughness))).rg;
    float3 specular = prefilteredColor * (F * brdf.x + brdf.y);
    
     
    float ambientFactor = max(0.2, dot(float3(0, 1, 0), -DirectionalLights[0].Direction.xyz));

    return (diffuse + specular) * ambientFactor;
}
/*******************************************************/

float3 ExpSqFog(float3 color, float3 ViewVector)
{
    float FogStart = 150.0f;
    float FogEnd = 350.0f;
    float3 FogColor = float3(0.5f, 0.6f, 0.7f);
    float3 SunColor = float3(1.0f, 0.9f, 0.7f);
    float FogThickness = 0.33f;
    
    if (length(ViewVector) < FogStart)
        return color;
    

    float3 lightDir = -normalize(mul((float3x3) View, DirectionalLights[0].Direction.xyz));
    float3 ViewDir = normalize(ViewVector);
    float fogDepth = (length(ViewVector) - FogStart) / (FogEnd - FogStart);
    float VdotL = max(dot(ViewDir, lightDir), 0.0);
    float3 fogColor = lerp(FogColor, SunColor, pow(VdotL, 8.0f));
    
    float fogFactor = exp(-fogDepth * fogDepth * FogThickness);
    color = lerp(fogColor, color, fogFactor);

    return color;
}


float4 main(PSInput input) : SV_Target0
{
    
    float4 packedAlbedoOpacity      = gAlbedo.Sample(AlbedoSampler, input.TexCoord);
    float4 packedDetail  = gRoughnessMetallic.Sample(RMSampler, input.TexCoord);
    
    // Get PBR Map values
    float4 Position     = UnpackPosition(input.TexCoord);
    float3 Normal       = UnpackNormal(input.TexCoord);
    float Roughness     = packedDetail.r;
    float Metallic      = packedDetail.g;
    float MaterialAO    = packedDetail.b;
    
    float3 Albedo = packedAlbedoOpacity.rgb;
    float Opacity = packedAlbedoOpacity.a;
    
    float4 Emissive         = gEmissive.Sample(EmissiveSampler, input.TexCoord);
    float AmbientOcclusion  = gHbao.Sample(HbaoSampler, input.TexCoord).r;

    float3 ViewVector = -Position.xyz;
    float3 ViewDir = normalize(ViewVector);
    
    // Use a base fresnel reflectance based on metallicity
    float3 Fresnel0 = lerp(float3(F0, F0, F0), Albedo, Metallic);
    
    /*--- Calculate Lights ---*/
    
    PBRData pbr;
    pbr.FragPos = Position.xyz;
    pbr.Normal = Normal;
    pbr.ViewDir = ViewDir;
    pbr.Albedo = Albedo;
    pbr.Roughness = Roughness;
    pbr.Metallic = Metallic;
    pbr.Fresnel0 = Fresnel0;
    pbr.LightDir = 0;
    pbr.AO = AmbientOcclusion;

    float3 brdfRadiance = 0;
    
    for (int i = 0; i < NumDLights; i++)
    {
        brdfRadiance += CalcDirectionalLight(DirectionalLights[i], pbr);
    }

    for (int j = 0; j < NumPLights; j++)
    {
        brdfRadiance += CalcPointLight(PointLights[j], pbr);
    }
    
    for (int k = 0; k < NumSLights; k++)
    {
        brdfRadiance += CalcSpotLight(SpotLights[k], pbr);
    }
    
    brdfRadiance += CalcAmbientLight(pbr);

    float3 FinalColor = brdfRadiance + Emissive.rgb * 5.0f;
    FinalColor = ExpSqFog(FinalColor, ViewVector);
      
    return float4(FinalColor, Opacity);

}
