
#include "Utilities/CookTorrance.hlsl"
#include "Utilities/Spaces.hlsl"


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
Texture2DArray gShadowmap : register(t4);
Texture2D gHbao : register(t5);
Texture2D gEmissive : register(t6);

Texture2D gBRDF_LUT : register(t7);

SamplerState DepthSampler : register(s0);
SamplerState NormalSampler : register(s1);
SamplerState AlbedoSampler : register(s2);
SamplerState RMSampler : register(s3);
SamplerState ShadowmapSampler : register(s4);
SamplerState HbaoSampler : register(s5);
SamplerState EmissiveSampler : register(s6);

SamplerState BRDFSampler : register(s7);

struct CascadeInfo
{
    matrix View;
    matrix Projection;
    float4 Depth;
};


struct PointLight
{
    float4 Position;
    float4 Color;
    float Intensity;
    float Range;
};


cbuffer CameraMatrices : register(b0)
{
    matrix View;
    matrix Projection;
}

cbuffer CascadeBuffer : register(b2)
{
    CascadeInfo DirLightCascades[SM_CASCADES];
}


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
    
}


cbuffer LightBuffer : register(b6)
{
    PointLight PointLights[4];
}

/*-------------*/


#define F0 0.04

static float maxSteps = 10;
static float maxDistance = 8;
static float resolution = 0.3;
static int steps = 5;
static float thickness = 0.5;
static float2 texSize = float2(2560, 1440);
static float strideZCutoff = 200;

static float nearZ = 0.1;
static float farZ = 300;

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
    return LinearizeDepth(gDepth.Sample(DepthSampler, tCoord).r, nearZ, farZ);
}


float GetLinearDepthTexelRaw(int2 pixel)
{
    return LinearizeDepth(gDepth.Load(int3(pixel, 0)).r, nearZ, farZ);
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

float ShadowCalculation(float4 fragPosLightSpace, float cascadeIndex)
{
    float4 shadowMapCoord = fragPosLightSpace / fragPosLightSpace.w;
    shadowMapCoord.y = -shadowMapCoord.y;
    shadowMapCoord.xy = shadowMapCoord.xy * 0.5 + 0.5;

    float depthCompare = shadowMapCoord.z;
    
    float2 dxdy = float2(1, 1) / float2(2048, 2048);
    float2 moments = gShadowmap.Sample(ShadowmapSampler, float3(shadowMapCoord.xy, (float)cascadeIndex)).xy;
    
    moments += gShadowmap.Sample(ShadowmapSampler, float3(shadowMapCoord.xy + float2(dxdy.x, 0), (float) cascadeIndex)).xy;
    moments += gShadowmap.Sample(ShadowmapSampler, float3(shadowMapCoord.xy + float2(dxdy.x, dxdy.y), (float) cascadeIndex)).xy;
    moments += gShadowmap.Sample(ShadowmapSampler, float3(shadowMapCoord.xy + float2(0, dxdy.y), (float) cascadeIndex)).xy;
    
    moments += gShadowmap.Sample(ShadowmapSampler, float3(shadowMapCoord.xy - float2(dxdy.x, 0), (float) cascadeIndex)).xy;
    moments += gShadowmap.Sample(ShadowmapSampler, float3(shadowMapCoord.xy - float2(dxdy.x, dxdy.y), (float) cascadeIndex)).xy;
    moments += gShadowmap.Sample(ShadowmapSampler, float3(shadowMapCoord.xy - float2(0, dxdy.y), (float) cascadeIndex)).xy;
    
    moments += gShadowmap.Sample(ShadowmapSampler, float3(shadowMapCoord.xy + float2(dxdy.x, -dxdy.y), (float) cascadeIndex)).xy;
    moments += gShadowmap.Sample(ShadowmapSampler, float3(shadowMapCoord.xy - float2(dxdy.x, -dxdy.y), (float) cascadeIndex)).xy;

    moments /= 9.0;
    
    return VarianceShadow(moments, depthCompare);
}


bool intersectsDepthBuffer(float z, float minZ, float maxZ)
{
    float depthScale = min(1.0f, z * strideZCutoff);
    z += thickness+ lerp(0.0f, 2.0f, depthScale);
    return (maxZ >= z) && (minZ - thickness <= z);
}

float distanceSquared(float2 a, float2 b)
{
    a -= b;
    return dot(a, a);
}


/*********************************************************/


bool traceScreenSpaceRay
    (
    // Camera-space ray origin, which must be within the view volume
    float3 csOrig,
    // Unit length camera-space ray direction
    float3 csDir,
    // Number between 0 and 1 for how far to bump the ray in stride units
    // to conceal banding artifacts. Not needed if stride == 1.
    float jitter,
    // Pixel coordinates of the first intersection with the scene
    out
    float2 hitPixel,
    // Camera space location of the ray hit
    out
    float3 hitPoint)
{
    // Clip to the near plane
        float rayLength = ((csOrig.z + csDir.z * maxDistance) < nearZ) ?
    (nearZ - csOrig.z) / csDir.z : maxDistance;
    
    
    float3 csEndPoint = csOrig + csDir * rayLength;

    // Project into homogeneous clip space
    float4 H0 = mul(Projection, float4(csOrig, 1.0f));
    H0.xyz /= H0.w;
    H0.y *= -1;
    H0.xy = H0.xy * 0.5 + 0.5;
    H0.xy *= texSize;
     
    float4 H1 = mul(Projection, float4(csEndPoint, 1.0f));
    H1.xyz /= H1.w;
    H1.y *= -1;
    H1.xy = H1.xy * 0.5 + 0.5;
    H1.xy *= texSize;
    
    float k0 = 1.0f / H0.w;
    float k1 = 1.0f / H1.w;

    // The interpolated homogeneous version of the camera-space points
    float3 Q0 = csOrig * k0;
    float3 Q1 = csEndPoint * k1;

    // Screen-space endpoints
    float2 P0 = H0.xy * k0;
    float2 P1 = H1.xy * k1;

    // If the line is degenerate, make it cover at least one pixel
    // to avoid handling zero-pixel extent as a special case later
    P1 += (distanceSquared(P0, P1) < 0.0001f) ? float2(0.01f, 0.01f) : 0.0f;
    float2 delta = P1 - P0;

    // Permute so that the primary iteration is in x to collapse
    // all quadrant-specific DDA cases later
    bool permute = false;
    if (abs(delta.x) < abs(delta.y))
    {
    // This is a more-vertical line
        permute = true;
        delta = delta.yx;
        P0 = P0.yx;
        P1 = P1.yx;
    }

    float stepDir = sign(delta.x);
    float invdx = stepDir / delta.x;

    // Track the derivatives of Q and k
    float3 dQ = (Q1 - Q0) * invdx;
    float dk = (k1 - k0) * invdx;
    float2 dP = float2(stepDir, delta.y * invdx);

    // Scale derivatives by the desired pixel stride and then
    // offset the starting values by the jitter fraction
    float strideScale = 1.0f - min(1.0f, abs(csOrig.z * strideZCutoff));
    float stride = 1.0f + strideScale * 1.2;
    dP *= stride;
    dQ *= stride;
    dk *= stride;

    P0 += dP * jitter;
    Q0 += dQ * jitter;
    k0 += dk * jitter;

    // Slide P from P0 to P1, (now-homogeneous) Q from Q0 to Q1, k from k0 to k1
    float4 PQk = float4(P0, Q0.z, k0);
    float4 dPQk = float4(dP, dQ.z, dk);
    float3 Q = Q0;

    // Adjust end condition for iteration direction
    float end = P1.x * stepDir;

    float stepCount = 0.0f;
    float prevZMaxEstimate = csOrig.z;
    float rayZMin = prevZMaxEstimate;
    float rayZMax = prevZMaxEstimate;
    float sceneZMax = rayZMax + 100.0f;
    for (;
    ((PQk.x * stepDir) <= end) && (stepCount < maxSteps) &&
    !intersectsDepthBuffer(sceneZMax, rayZMin, rayZMax) &&
    (sceneZMax != 0.0f);
    ++stepCount)
    {
        rayZMin = prevZMaxEstimate;
        rayZMax = (dPQk.z * 0.5f + PQk.z) / (dPQk.w * 0.5f + PQk.w);
        prevZMaxEstimate = rayZMax;
        if (rayZMin > rayZMax)
        {
            swap(rayZMin, rayZMax);
        }

        hitPixel = permute ? PQk.yx : PQk.xy;
    // You may need hitPixel.y = depthBufferSize.y - hitPixel.y; here if your vertical axis
    // is different than ours in screen space
        sceneZMax = GetLinearDepthTexelRaw(int2(hitPixel));

        PQk += dPQk;
    }

    // Advance Q based on the number of steps
    Q.xy += dQ.xy * stepCount;
    hitPoint = Q * (1.0f / PQk.w);
    return intersectsDepthBuffer(sceneZMax, rayZMin, rayZMax);
}
float4 SSR2(float2 FragTexCoord, float4 ViewPos, float3 Normal)
{
    float3 unitPositionFrom = normalize(ViewPos.xyz);
    float3 pivot = normalize(reflect(unitPositionFrom, Normal));
    
    float rDotV = dot(unitPositionFrom, pivot);
    
    
    float2 hitPixel = float2(0.0f, 0.0f);
    float3 hitPoint = float3(0.0f, 0.0f, 0.0f);
    
    float jitter = thickness > 1.0f ? float(int(FragTexCoord.x + FragTexCoord.y) & 1) * 0.5f : 0.0f;
    
    bool intersection = traceScreenSpaceRay(ViewPos.xyz, pivot, jitter, hitPixel, hitPoint);
    float depth = gDepth.Load(int3(hitPixel, 0)).r;
    
    hitPixel /= texSize;
    if (hitPixel.x > 1.0f || hitPixel.x < 0.0f || hitPixel.y > 1.0f || hitPixel.y < 0.0f)
    {
        intersection = false;
    }

    
    return float4(hitPixel, rDotV, depth) * (intersection ? 1.0f : 0.0f);

}

float4 SSR(float2 FragTexCoord, float4 ViewPos, float3 Normal)
{
    
    float4 uv = 0;
    
    float2 texCoord = FragTexCoord / texSize;


    if (ViewPos.w <= 0.0)
    {
        return uv;
    }

    float3 unitPositionFrom = normalize(ViewPos.xyz);
    float3 pivot = normalize(reflect(unitPositionFrom, Normal));

    float4 positionTo = ViewPos;

    float4 startView = float4(ViewPos.xyz + (pivot * 0.0), 1.0);
    float4 endView = float4(ViewPos.xyz + (pivot * maxDistance), 1.0);

    float4 startFrag = startView;
    startFrag = mul(Projection, startFrag);
    startFrag.xyz /= startFrag.w;
    startFrag.y *= -1;
    startFrag.xy = startFrag.xy * 0.5 + 0.5;
    startFrag.xy *= texSize;

    float4 endFrag = endView;
    endFrag = mul(Projection, endFrag);
    endFrag.xyz /= endFrag.w;
    endFrag.y *= -1;
    endFrag.xy = endFrag.xy * 0.5 + 0.5;
    endFrag.xy *= texSize;

    float2 frag = startFrag.xy;
    uv.xy = frag / texSize;

    float deltaX = endFrag.x - startFrag.x;
    float deltaY = endFrag.y - startFrag.y;
    float useX = abs(deltaX) >= abs(deltaY) ? 1.0 : 0.0;
    float delta = lerp(abs(deltaY), abs(deltaX), useX) * resolution;
    float2 increment = float2(deltaX, deltaY) / max(delta, 0.001);

    float search0 = 0;
    float search1 = 0;

    int hit0 = 0;
    int hit1 = 0;

    float viewDistance = -startView.z;
    float depth = thickness;

    float i = 0;

    int maxDelta = clamp(int(delta), 0, 300);
    
    for (i = 0; i < maxDelta; ++i)
    {
        frag += increment;
        uv.xy = frag / texSize;

        float compareDepth = GetLinearDepth(uv.xy);
        
        search1 =
      lerp
        ((frag.y - startFrag.y) / deltaY
        , (frag.x - startFrag.x) / deltaX
        , useX
        );

        search1 = clamp(search1, 0.0, 1.0);

        viewDistance = -(startView.z * endView.z) / lerp(endView.z, startView.z, search1);
        depth = viewDistance - compareDepth;

        if (depth > 0 && depth < thickness)
        {
            hit0 = 1;
            break;
        }
        else
        {
            search0 = search1;
        }
    }

    search1 = search0 + ((search1 - search0) / 2.0);

    int raySteps = steps * hit0;

    for (i = 0; i < raySteps; ++i)
    {
        frag = lerp(startFrag.xy, endFrag.xy, search1);
        uv.xy = frag / texSize;
        
        float compareDepth = GetLinearDepth(uv.xy);

        viewDistance = -(startView.z * endView.z) / lerp(endView.z, startView.z, search1);
        depth = viewDistance - compareDepth;

        if (depth > 0 && depth < thickness)
        {
            hit1 = 1;
            search1 = search0 + ((search1 - search0) / 2);
        }
        else
        {
            float temp = search1;
            search1 = search1 + ((search1 - search0) / 2);
            search0 = temp;
        }
    }

    float visibility =
      hit1
    * positionTo.w
    * (1
      - max
         (dot(-unitPositionFrom, pivot)
         , 0
         )
      )
    * (1
      - clamp
          (depth / thickness
          , 0
          , 1
          )
      )
    * (1
      - clamp
          (length(positionTo - ViewPos)
            / maxDistance
          , 0
          , 1
          )
      )
    * (uv.x < 0 || uv.x > 1 ? 0 : 1)
    * (uv.y < 0 || uv.y > 1 ? 0 : 1);

    visibility = clamp(visibility, 0, 1);

    uv.ba = visibility;
    
    return uv;
    
}


float4 raytrace(in float3 reflectionVector, in float startDepth, float2 texCoord)
{
    float stepSize = 0.3;

    float size = length(reflectionVector.xy);
    reflectionVector = normalize(reflectionVector / size);
    reflectionVector = reflectionVector * stepSize;
        
        // Current sampling position is at current fragment
    float2 sampledPosition = texCoord;
        // Current depth at current fragment
    float currentDepth = startDepth;
        // The sampled depth at the current sampling position
    float sampledDepth = GetLinearDepth(sampledPosition);

    
    float2 reflectionUv = 0;
    float visibility = 0;
    
    
        // Raytrace as long as in texture space of depth buffer (between 0 and 1)
    [loop]
    while (sampledPosition.x <= 1.0 && sampledPosition.x >= 0.0 &&
	      sampledPosition.y <= 1.0 && sampledPosition.y >= 0.0)
    {
                // Update sampling position by adding reflection vector's xy and y components
        sampledPosition = sampledPosition + reflectionVector.xy;
                // Updating depth values
        currentDepth = currentDepth + reflectionVector.z * startDepth;
        float sampledDepth = GetLinearDepth(sampledPosition);
                
                // If current depth is greater than sampled depth of depth buffer, intersection is found
        if (currentDepth > sampledDepth)
        {
                        // Delta is for stop the raytracing after the first intersection is found
                        // Not using delta will create "repeating artifacts"
            float delta = (currentDepth - sampledDepth);
            if (delta < 0.003f)
            {
                reflectionUv = sampledPosition;
                visibility = 1;
                break;
            }
        }
    }

    return float4(reflectionUv, 1,  0);
}
float4 SSR3(float2 FragTexCoord, float4 ViewPos, float3 Normal)
{

    float3 unitPositionFrom = normalize(ViewPos.xyz);
    float3 pivot = normalize(reflect(unitPositionFrom, Normal));

	// Depth at current fragment
    float currDepth = GetLinearDepth(FragTexCoord);

	// Eye position, camera is at (0, 0, 0), we look along negative z, add near plane to correct parallax
    float3 eyePosition = normalize(float3(0, 0, nearZ));
    float4 reflectionVector = mul(Projection, reflect(float4(-eyePosition, 0), float4(Normal, 0)));

        // Call raytrace to get reflected color
    float4 reflectedUvs = raytrace(reflectionVector.xyz, currDepth, FragTexCoord);


    return reflectedUvs;
}

float2 BinarySearch(inout float3 dir, inout float3 hitCoord, inout float dDepth)
{
    float depth;

    float4 projectedCoord;

    for (int i = 0; i < 10; i++)
    {
        projectedCoord = mul(Projection, float4(hitCoord, 1.0));
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.y *= -1;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;

        depth = GetLinearDepth(projectedCoord.xy);

        dDepth = abs(hitCoord.z) - depth;

        dir *= 0.5;
        if (dDepth > 0.0)
            hitCoord += dir;
        else
            hitCoord -= dir;
    }

    projectedCoord = mul(Projection, float4(hitCoord, 1.0));
    projectedCoord.xy /= projectedCoord.w;
    projectedCoord.y *= -1;
    projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;

    return float2(projectedCoord.xy);
}
float2 rayCast(float3 dir, inout float3 hitCoord)
{
    
    dir *= 0.25;

    for (int i = 0; i < 30; i++)
    {
        hitCoord += dir;

        float4 projectedCoord = mul(Projection, float4(hitCoord, 1.0));
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.y *= -1;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;

        float depth = GetLinearDepth(projectedCoord.xy);
        float dDepth = abs(hitCoord.z) - depth;

        if (dDepth >= 0.0)
            return BinarySearch(dir, hitCoord, dDepth);
    }

    return float2(-1.0, -1.0);
}
float4 SSR4(float2 FragTexCoord, float4 ViewPos, float3 Normal)
{
    float3 unitPositionFrom = normalize(ViewPos.xyz);
    float3 pivot = normalize(reflect(unitPositionFrom, Normal));
    
    float3 hitCoord = ViewPos.xyz;
    
    float2 uv = rayCast(pivot * max(0.1, -ViewPos.z), hitCoord);
    
    
    float visibility =
    (1
      - max
         (dot(-unitPositionFrom, pivot)
         , 0
         )
      ) 
    * 
    (1
      - clamp
          (length(hitCoord - ViewPos.xyz)
            / maxDistance
          , 0
          , 1
          )
      )
    * (uv.x < 0 || uv.x > 1 ? 0 : 1)
    * (uv.y < 0 || uv.y > 1 ? 0 : 1);
    
    return float4(uv, visibility, 1);
}


float maxReflDistance = 300.0;

float4 SSR5(float2 FragTexCoord, float4 ViewPos, float3 Normal)
{

    
    float3 unitPositionFrom = normalize(ViewPos.xyz);
    float3 pivot = normalize(reflect(unitPositionFrom, Normal));
    
    float3 hitCoord = ViewPos.xyz;
    
    float2 uv = float2(-1, -1);
    
    float increment = 0.25; 
    float3 dir = pivot * increment;

    for (int i = 0; i < 40; i++)
    {
        hitCoord += dir;

        float4 projectedCoord = mul(Projection, float4(hitCoord, 1.0));
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.y *= -1;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;

        float depth = GetLinearDepth(projectedCoord.xy);
        float dDepth = abs(hitCoord.z) - abs(depth);

        if (dDepth >= 0.0)
        {
        
            projectedCoord = mul(Projection, float4(hitCoord, 1.0));
            projectedCoord.xy /= projectedCoord.w;
            projectedCoord.y *= -1;
            projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
            
            uv = projectedCoord.xy;
            
            break;
        }
    }
    
    float visibility =
    (1
      - max
         (dot(-unitPositionFrom, pivot)
         , 0
         )
      )
    /**
    (1
      - clamp
          (length(hitCoord - ViewPos.xyz)
            / maxDistance
          , 0
          , 1
          )
      )*/
    * (uv.x < 0 || uv.x > 1 ? 0 : 1)
    * (uv.y < 0 || uv.y > 1 ? 0 : 1);
   
    
    return float4(uv, visibility, 1);

}

/*******************************************************/



float3 CalcPointLight(PointLight pl, float3 FragPos, float3 Normal, float3 ViewDir, float3 Albedo, float Roughness, float Metallic, float3 Fresnel0)
{
    // reflectance equation

    // calculate per-light radiance
    
    
    float3 LightPosViewSpace = mul(View, pl.Position).xyz;
    
    float3 LightDir = normalize(LightPosViewSpace - FragPos);
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

    
    // Calculate point light attenuation
    float distance = length(LightPosViewSpace - FragPos);
    float attDenom = (distance / max(pl.Range, 3.0) + 1);
    float attenuation = pl.Intensity / (attDenom * attDenom);
    float3 radianceIn = pl.Color * attenuation * NdotL;
    
 
    // add to outgoing radiance Lo
    float3 radiance = (kD * Albedo / PI + specular) * radianceIn; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    return radiance;
}

float3 CalcDirectionalLight(float3 FragPos, float3 Normal, float3 ViewDir, float3 Albedo, float Roughness, float Metallic, float3 Fresnel0)
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
    
    
    
    int cascadeIdx = 0;
    float fDepth = -FragPos.z;
    
    for (int i = 0; i < SM_CASCADES; ++i)
    {
        float cascadeViewDepthFar = DirLightCascades[i].Depth.r;
        
        if (fDepth < cascadeViewDepthFar)
        {
            cascadeIdx = i;
            break;
        }
    }
    
    

    matrix dirLightVp = mul(DirLightCascades[cascadeIdx].Projection, DirLightCascades[cascadeIdx].View);
    float4 fragPosWorldSpace = mul(InverseView, float4(FragPos, 1.0));
    float4 fragPosLightSpace = mul(dirLightVp, fragPosWorldSpace);
    
    float Shadow = ShadowCalculation(fragPosLightSpace, cascadeIdx);
    
    
    
    return radiance * Shadow;
}


/*******************************************************/


float4 main(PSInput input) : SV_Target0
{
    
    float4 packedAlbedoOpacity = gAlbedo.Sample(AlbedoSampler, input.TexCoord);
    float4 packedRoughnessMetallic = gRoughnessMetallic.Sample(RMSampler, input.TexCoord);
    
    // Get PBR Map values
    float4 FragPos = float4(UnpackPosition(input.TexCoord), 1.0f);
    float3 Normal = UnpackNormal(input.TexCoord);
    float3 Albedo = packedAlbedoOpacity.rgb;
    float Roughness = packedRoughnessMetallic.r;
    float Metallic = packedRoughnessMetallic.g;
    float Opacity = packedAlbedoOpacity.a;
    float4 Emissive = gEmissive.Sample(EmissiveSampler, input.TexCoord);
    float AmbientOcclusion = gHbao.Sample(HbaoSampler, input.TexCoord);
    //float3 ReflectedPos = normalize(reflect(FragPos.xyz, Normal));
   
    
    
    // Use a base fresnel reflectance based on metallicity
    float3 Fresnel0 = float3(F0, F0, F0);
    Fresnel0 = lerp(Fresnel0, Albedo, Metallic);
    
    
    // reflectance equation
    float3 ViewDir = -normalize(FragPos.rgb);
    float3 totalRadiance = CalcDirectionalLight(FragPos.xyz, Normal, ViewDir, Albedo, Roughness, Metallic, Fresnel0);


    for (int i = 0; i < 4;i++)
    {
        totalRadiance += CalcPointLight(PointLights[i],FragPos.xyz, Normal, ViewDir, Albedo, Roughness, Metallic, Fresnel0);
    }
    
    
    
    /* ---------------*/
        
    float3 F = fresnelSchlickRoughness(max(dot(Normal, ViewDir), 0.0), Fresnel0, Roughness);
    
    float kS = F;
    float kD = 1.0 - kS;
    kD *= 1.0 - Metallic;
    
    
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    
    
    float4 ssrCoords = 0;//SSR5(input.TexCoord, FragPos, Normal);

    float3 prefilteredColor = gAlbedo.SampleLevel(AlbedoSampler, ssrCoords.xy, Roughness * MAX_REFLECTION_LOD);
    float2 brdf = gBRDF_LUT.Sample(BRDFSampler, float2(max(dot(Normal, ViewDir), 0.0), 1.0-Roughness)).rg;
    float3 specular = prefilteredColor * (F * brdf.x + brdf.y) * ssrCoords.b;

    float3 Ambient = 1.0 * (kD * Albedo + specular) * AmbientOcclusion;
    
    /*-------------------------*/
    
    

    
    // Compute Final Color
    float3 FinalColor = Ambient + totalRadiance + Emissive.rgb * Emissive.a;
 
    return float4(FinalColor, Opacity);

}