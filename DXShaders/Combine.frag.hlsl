
struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};


Texture2D gDepth : register(t0);
Texture2D gNormal : register(t1);
Texture2D gAlbedo : register(t2);
Texture2D gSpecular : register(t3);
Texture2DArray gShadowmap : register(t4);
Texture2D gHbao : register(t5);

SamplerState DepthSampler : register(s0);
SamplerState NormalSampler : register(s1);
SamplerState AlbedoSampler : register(s2);
SamplerState SpecularSampler : register(s3);
SamplerState ShadowmapSampler : register(s4);
SamplerState HbaoSampler : register(s5);

struct CascadeInfo
{
    matrix View;
    matrix Projection;
    float4 Depth;
};


cbuffer CameraMatrices : register(b0)
{
    matrix View;
    matrix Projection;
}

cbuffer CascadeBuffer : register(b2)
{
    CascadeInfo DirLightCascades[5];
}


cbuffer CombineBuffer : register(b4)
{
    matrix InverseView;
    matrix InverseProjection;
    float4 DirLightDirection;
    float4 DirLightInfo;
}


/* ------------------------------------------------------------------------------------ */

float3 UnpackNormal(float2 tCoord)
{
    return normalize(gNormal.Sample(NormalSampler, tCoord).rgb * 2 - 1.0f);
}

float GetDepth(float2 tCoord)
{
    return gDepth.Sample(DepthSampler, tCoord).r;
}

float3 UnpackPosition(float2 tCoord)
{ 
    float z = GetDepth(tCoord);
    
    float x = tCoord.x * 2.0f - 1.0f;
    float y = (1 - tCoord.y) * 2 - 1.0f;
    
    float4 tRay = mul(InverseProjection, float4(x, y, z, 1.0f));
    return tRay.xyz / tRay.w;
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

        return pow(pMax, 4);
    }
}

float PcfShadow(float2 moments, float compare)
{
    if (compare - 0.01f>= moments.x)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


float ShadowCalculation(float4 fragPosLightSpace, float cascadeIndex)
{
    float4 shadowMapCoord = fragPosLightSpace / fragPosLightSpace.w;
    shadowMapCoord.y = -shadowMapCoord.y;
    shadowMapCoord.xy = shadowMapCoord.xy * 0.5 + 0.5;

    float depthCompare = shadowMapCoord.z;
    float2 moments = gShadowmap.Sample(ShadowmapSampler, float3(shadowMapCoord.xy, (float)cascadeIndex)).xy;

    return VarianceShadow(moments, depthCompare);
}


float4 main(PSInput input) : SV_Target0
{
    float4 FragPos = float4(UnpackPosition(input.TexCoord), 1.0f);
    float3 Normal = UnpackNormal(input.TexCoord);
    float4 fullAlbedo = gAlbedo.Sample(AlbedoSampler, input.TexCoord);
    
    float3 Albedo = fullAlbedo.rgb;
    float AmbientOcclusion = gHbao.Sample(HbaoSampler, input.TexCoord);
    float3 ambient = float3(0.3f * Albedo * AmbientOcclusion);
    
    float3 lightDir = -normalize(mul((float3x3) View, DirLightDirection.rgb));
    float diffFactor = max(dot(lightDir, Normal), 0.0f);
    float3 diffuse = Albedo * diffFactor * AmbientOcclusion * DirLightInfo.w;
    
    
    
    int cascadeIdx = 0;
    float fDepth = abs(FragPos.z);
    
    for (int i = 0; i < 5; ++i)
    {
        float cascadeViewDepthFar = DirLightCascades[i].Depth.r;
        
        if (fDepth < cascadeViewDepthFar)
        {
            cascadeIdx = i;
            break;
        }
    }
    
    /*
    float4 farDepths = float4(  DirLightCascades[0].Depth.r,
                                DirLightCascades[1].Depth.r,
                                DirLightCascades[2].Depth.r,
                                DirLightCascades[3].Depth.r
);
    
    float4 farDepths2 = float4( DirLightCascades[4].Depth.r,
                                    0,
                                    0,
                                    0
                                );
    

    float4 comparison = (fDepth > farDepths);
    float4 comparison2 = (fDepth > farDepths2);
    
    float fIndex = dot(comparison, float4(1, 1, 1, 1)) + dot(comparison2, float4(1, 0, 0, 0));
    
    cascadeIdx = (int) fIndex;
    */
    

    matrix dirLightVp = mul(DirLightCascades[cascadeIdx].Projection, DirLightCascades[cascadeIdx].View);
    float4 fragPosWorldSpace = mul(InverseView, FragPos);
    float4 fragPosLightSpace = mul(dirLightVp, fragPosWorldSpace);
    
    float shadow = ShadowCalculation(fragPosLightSpace, cascadeIdx);
    
    
   /* float3 UpperHemisphere = float3(1, 0, 0);
    float3 LowerHeimsphere = float3(0, 0.1,1);
    float3 Up = mul(View, float4(0, 1, 0, 0)).xyz;
    
    float w = 0.5 * (1 + dot(Up, Normal));
    float HemisphereColor = (w * UpperHemisphere) + (1.0 - w) * LowerHeimsphere;*/
    
    
    float3 FinalColor = ambient + shadow * diffuse;
    float3 ToneMappedColor = FinalColor / (FinalColor + float3(1, 1, 1));
    
    
    float Gamma = 2.2;
    float InvGamma = 1.0f / Gamma;
    
    //float3 GammaCorrectedColor = pow(ToneMappedColor, float3(InvGamma, InvGamma, InvGamma));
    
    //return float4(GammaCorrectedColor, fullAlbedo.a);
    return float4(ToneMappedColor, fullAlbedo.a);

}