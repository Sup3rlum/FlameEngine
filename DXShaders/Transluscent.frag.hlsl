
#include "Utilities/CookTorrance.hlsl"

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
    
    float3 ViewPos : VIEWPOS0;
	
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

cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};


cbuffer DirLightBuffer : register(b2)
{
    float4 DirLightDirection;
    float4 DirLightInfo;
}

#define F0 0.04

float3 CalcDirectionalLight(float3 Normal, float3 ViewDir, float3 Albedo, float Roughness, float Metallic, float3 Fresnel0)
{
    float3 LightDir = -normalize(mul((float3x3) View, DirLightDirection.rgb));
    float3 HalfDir = normalize(ViewDir + LightDir);

    float NdotL = max(dot(Normal, LightDir), 0.0);
    
        // Cook-Torrance BRDF
    float NDF = DistributionGGX(Normal, HalfDir, Roughness);
    float G = GeometrySmith(Normal, ViewDir, LightDir, Roughness);
    float3 F = fresnelSchlick(max(dot(HalfDir, ViewDir), 0.0), Fresnel0);
           
       
    float3 numerator = NDF * G * F;
    float denominator = 4 * max(dot(Normal, ViewDir), 0.0) * NdotL + 0.0001; // + 0.0001 to prevent divide by zero
    float3 specular = numerator / denominator;
        
    float3 kS = F;
    float3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;


    float3 radianceIn = DirLightInfo.rgb * DirLightInfo.a * NdotL;
    
    // add to outgoing radiance Lo
    float3 radiance = (kD * Albedo / PI + specular) * radianceIn; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    
    return radiance;
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
float4 main(PSInput input) : SV_TARGET
{
    
    float4 packedAlbedoOpacity = DiffuseMap.Sample(DiffuseSampler, input.TexCoord);
   
    // Get PBR Map values
    float3 Normal = CalculateNormalMap(input, input.TexCoord);
    float3 Albedo = packedAlbedoOpacity.rgb;
    float Roughness = RoughnessMap.Sample(RoughnessSampler, input.TexCoord).r;
    float Metallic = MetallicMap.Sample(MetallicSampler, input.TexCoord).r;
    float MaterialAO = AOMap.Sample(AOSampler, input.TexCoord).r;
    float Opacity = packedAlbedoOpacity.a;


    
    // Use a base fresnel reflectance based on metallicity
    float3 Fresnel0 = float3(F0, F0, F0);
    Fresnel0 = lerp(Fresnel0, Albedo, Metallic);
    
    
    // reflectance equation
    float3 ViewDir = -normalize(input.ViewPos.rgb);
    float3 totalRadiance = CalcDirectionalLight(Normal, ViewDir, Albedo, Roughness, Metallic, Fresnel0);

    
    /* ---------------*/
        
    float3 F = fresnelSchlickRoughness(max(dot(Normal, ViewDir), 0.0), Fresnel0, Roughness);
    
    float kS = F;
    float kD = 1.0 - kS;
    kD *= 1.0 - Metallic;
    
    
    float3 Ambient = 1.0 * (kD * Albedo) * MaterialAO;
    
    /*-------------------------*/
    
   
    
    // Compute Final Color
    float3 FinalColor = Ambient + totalRadiance;
    return float4(FinalColor, Opacity);
}