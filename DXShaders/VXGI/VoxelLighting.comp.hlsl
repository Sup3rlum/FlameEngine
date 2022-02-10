
#include "../Utilities/Packing.hlsl"
#include "../Utilities/LightData.hlsl"

#include "VXGIBuffers.hlsl"

RWTexture3D<uint> g_VoxelAlbedo : register(u0);
RWTexture3D<uint> g_VoxelNormal : register(u1);
RWTexture3D<uint> g_VoxelDetail : register(u2);


RWTexture3D<unorm float4> g_VoxelLit : register(u3);



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


#define MAX_LIGHTS 8

cbuffer DirectionalLightData : register(b6)
{
    DirectionalLight DirectionalLights[MAX_LIGHTS];
}
cbuffer PointLightData : register(b7)
{
    PointLight PointLights[MAX_LIGHTS];
}
cbuffer SpotLightData : register(b8)
{
    SpotLight SpotLights[MAX_LIGHTS];
}


float3 UnpackNormal(float3 pNormal)
{
    return normalize(pNormal * 2 - 1.0f);
}


static float traceShadowHit = 0.5f;

float TraceShadow(float3 position, float3 direction, float maxTracingDistance)
{
    // scaling factor
    float k = traceShadowHit * traceShadowHit;
    // navigation
    float voxelTexSize = 1.0f / voxelVolumeDimension;
    // move one voxel further to avoid self collision
    float dst = voxelTexSize * 2.0f;
    float3 samplePos = direction * dst + position;
    // control variables
    float visibility = 0.0f;
    // accumulated sample
    float traceSample = 0.0f;

    while (visibility <= 1.0f && dst <= maxTracingDistance)
    {
        if (samplePos.x < 0.0f || samplePos.y < 0.0f || samplePos.z < 0.0f
            || samplePos.x > 1.0f || samplePos.y > 1.0f || samplePos.z > 1.0f)
        {
            break;
        }
        
        
        float4 tmpSample = UintToRGBA8(g_VoxelAlbedo[samplePos * voxelVolumeDimension]) / float4(255.0, 255, 255, 1);    
        traceSample = ceil(tmpSample.a) * k;

        // hard shadows mode
        if (traceSample > 1.0f - 1e-30)
        {
            return 0.0f;
        }

        // accumulate
        visibility += (1.0f - visibility) * traceSample / dst;
        // move further into volume
        dst += voxelTexSize;
        samplePos = direction * dst + position;
    }

    return 1.0f - visibility;
}


float3 CalcDirectional(DirectionalLight dLight, float3 FragPos, float3 Normal, float3 Albedo)
{
    float3 LightDir = -normalize(dLight.Direction.xyz);
    float NdotL = max(0, dot(LightDir, Normal));
    
    float3 voxelPos = WorldToVoxel(FragPos);
    float Shadow = TraceShadow(voxelPos, LightDir, 1.0f);

         
    float3 radianceIn = dLight.Radiance.rgb * dLight.Radiance.a * Shadow;
    float3 radianceOut = Albedo * NdotL * radianceIn;
         
    return radianceOut;
}


float3 CalcPointLight(PointLight pLight, float3 FragPos, float3 Normal, float3 Albedo)
{
    float3 LightDir = normalize(pLight.Position.xyz - FragPos);
    float NdotL = max(0, dot(Normal, LightDir));
    
    // Calculate point light attenuation
    float distance = length(pLight.Position.xyz - FragPos);
    float attDenom = (distance / pLight.Radius + 1.0);
    float attenuation = pLight.Radiance.a / (attDenom * attDenom);
    float3 radianceIn = pLight.Radiance.rgb * attenuation;
    
    // Multiply radiance coeff by BRDF factor
    float3 radianceOut = Albedo * NdotL* radianceIn;
    return radianceOut;
}


float3 CalcSpotLight(SpotLight sLight, float3 FragPos, float3 Normal, float3 Albedo)
{
    float3 LightDir = normalize(sLight.Position.xyz - FragPos);
    float NdotL = max(0, dot(Normal, LightDir));
    
    // Calculate spot light aperture incidence
    float DdotL = dot(sLight.Direction.xyz, -LightDir);
    float apertureFactor = DdotL >= sLight.ApertureSize;
    
    // Calculate point light attenuation
    float distance = length(sLight.Position.xyz - FragPos);
    float attDenom = (distance / sLight.Radius + 1.0);
    float attenuation = sLight.Radiance.a / (attDenom * attDenom);
    float3 radianceIn = sLight.Radiance.rgb * attenuation * apertureFactor;
    
    // Multiply radiance coeff by BRDF factor
    float3 radianceOut = Albedo * NdotL* radianceIn;
    return radianceOut;
}




float3 CalculateRadiance(float3 position, float3 normal, float3 albedo)
{
    float3 totalRadiance = 0;
    
    
    
    for (int i = 0; i < NumDLights;i++)
    {  
        totalRadiance += CalcDirectional(DirectionalLights[i], position, normal, albedo);
    }
    for (int j = 0; j < NumPLights;j++)
    {
        totalRadiance += CalcPointLight(PointLights[j], position, normal, albedo);
    }
    for (int k = 0; k < NumSLights; k++)
    {
        totalRadiance += CalcSpotLight(SpotLights[k], position, normal, albedo);
    }
   
    totalRadiance += albedo * 0.01f;
    
    return totalRadiance;
}



[numthreads(8, 8, 8)]
void main( uint3 DTid : SV_DispatchThreadID )
{

    float3 position = VoxelToWorld(DTid);
    float4 albedo = UintToRGBA8(g_VoxelAlbedo[DTid]) / float4(255.0, 255, 255, 1);
    float3 normal = UnpackNormal(UintToRGBA8(g_VoxelNormal[DTid]).rgb / 255.0);
    
 
    float3 totalRadiance = CalculateRadiance(position, normal, albedo.rgb);
    
    g_VoxelLit[DTid] = float4(totalRadiance, albedo.a);
    
}