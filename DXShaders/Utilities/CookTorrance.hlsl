
#define PI 3.141592

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
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
    return F0 + (1.0 - F0) * clamp(1.0 - cosTheta, 0.0, 1.0) * tmp * tmp * tmp * tmp * tmp;
}
// ----------------------------------------------------------------------------

float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    float tmp = clamp(1.0 - cosTheta, 0.0, 1.0);
    return F0 + (max(1.0 - roughness, F0) - F0) * tmp * tmp * tmp * tmp * tmp;
}
// ----------------------------------------------------------------------------


float3 BRDF(
            float3 FragPos, 
            float3 Normal, 
            float3 ViewDir, 
            float3 LightDir,
            float3 Albedo, 
            float Roughness, 
            float Metallic, 
            float3 Fresnel0
            )
{
    
    
    float3 HalfDir = normalize(ViewDir + LightDir);
    float NdotL = max(dot(Normal, LightDir), 0.0);
    
    // Cook-Torrance BRDF
    float NDF   = DistributionGGX(Normal, HalfDir, Roughness);
    float G     = GeometrySmith(Normal, ViewDir, LightDir, Roughness);
    float3 F    = fresnelSchlick(max(dot(HalfDir, ViewDir), 0.0), Fresnel0);
           
       
    float3 numerator = NDF * G * F;
    float denominator = 4 * max(dot(Normal, ViewDir), 0.0) * NdotL + 0.0001;
    float3 specular = numerator / denominator;
        
    float3 kS = F;
    float3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;
    
    float3 radiance = (kD * Albedo / PI + specular) * NdotL; 
    
    return radiance;
}