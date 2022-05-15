
#define PI 3.141592

struct PBRData
{
    float3 FragPos;
    float3 Normal;
    float3 Albedo;
    float3 ViewDir;
    float3 LightDir;
    float Roughness;
    float Metallic;
    float3 Fresnel0;
    float AO;
};

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2 * NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
float3 fresnelSchlick(float cosTheta, float3 F0)
{
    float tmp = clamp(1.0 - cosTheta, 0.0, 1.0);
    return F0 + (1.0 - F0) * tmp * tmp * tmp * tmp * tmp;
}
// ----------------------------------------------------------------------------

float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    float tmp = clamp(1.0 - cosTheta, 0.0, 1.0);
    return F0 + (max(1.0 - roughness, F0) - F0) * tmp * tmp * tmp * tmp * tmp;
}
// ----------------------------------------------------------------------------


float3 BRDF(PBRData pbr) 
{   
    float3 HalfDir = normalize(pbr.ViewDir + pbr.LightDir);
    float NdotL = max(dot(pbr.Normal, pbr.LightDir), 0.0);
    float NdotV = max(dot(pbr.Normal, pbr.ViewDir), 0.0);
    float HdotV = max(dot(HalfDir, pbr.ViewDir), 0.0);
    float HdotN = max(dot(HalfDir, pbr.Normal), 0.0);
    
    // Cook-Torrance BRDF
    float NDF   = DistributionGGX(pbr.Normal, HalfDir, pbr.Roughness);
    float G     = GeometrySmith(pbr.Normal, pbr.ViewDir, pbr.LightDir, pbr.Roughness);
    float3 F    = fresnelSchlick(HdotV, pbr.Fresnel0);
           
       
    float3 numerator = NDF * G * F;
    float denominator = 4 * NdotV * NdotL + 0.0001;
    float3 specular = numerator / denominator;
        
    float3 kD = (1.0 - F) * (1.0 - pbr.Metallic);
    float3 diffuse = kD * pbr.Albedo * pbr.AO / PI;
    
    float3 radiance = (diffuse + specular) * NdotL; 
    return radiance;
}
