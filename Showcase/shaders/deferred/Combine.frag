#version 450


out vec4 FragColor;

in vec2 fTexCoord;



/* --------------- Uniforms --------------------*/

layout(binding=0) uniform sampler2D gDepth;
layout(binding=1) uniform sampler2D gNormal;
layout(binding=2) uniform sampler2D gAlbedo;
layout(binding=3) uniform sampler2D gSpecular;
layout(binding=4) uniform sampler2DArray gShadowmap;
layout(binding=5) uniform sampler2D gHbao;

uniform mat4x4 InverseProjection;
uniform mat4x4 InverseView;
uniform mat4x4 View;

/*----------------------------------------------*/


struct DirectionalLightCascadeInfo
{
	mat4x4 VPMatrix;
	float Depth;
};

struct DirectionalLight
{
	vec3 Direction;
	vec4 Color;
	float Intensity;
	DirectionalLightCascadeInfo Info[5];
};
struct PointLight
{
	vec3 Position;
	float Intensity;
	float Radius;
	vec4 Color;
};


/*---------------- Light Data ---------------------*/

uniform DirectionalLight DirectionalLights[5];
uniform PointLight PointLights[16];

uniform int NumLights = 1;

/*-------------------------------------------------*/



float MinVariance = 0.000001f;



/* ------------------ Util Funcs -------------------------*/

vec3 UnpackNormal(vec2 tCoord)
{
	return normalize(texture(gNormal,tCoord).rgb * 2.0 - 1.0);
}
float GetDepth(vec2 tCoord)
{
	return texture(gDepth, tCoord).r * 2.0 - 1.0;
}

vec3 UnpackPosition(vec2 tCoord)
{
	float d = GetDepth(tCoord);

	vec4 tray = InverseProjection * vec4(tCoord * 2.0 - 1.0, d, 1.0);

	return tray.xyz / tray.w;
}

/*----------------- Shadow Funcs --------------------*/


float VarianceShadow(vec2 moments, float compare)
{

	float p = step(compare, moments.x);
	float variance = max(moments.y - moments.x * moments.x, MinVariance);

	float d = compare - moments.x;
	float pMax = smoothstep(0.0, 1.0, variance / (variance + d * d));


	return min(max(p, pMax), 1.0);
}
float PcfShadow(sampler2DArray shadowMap, float cascadeIndex, vec2 coords, float compare)
{
	vec3 fNormal =  texture(gNormal, fTexCoord).rgb * 2.0 - 1.0;

    float closestDepth = texture(gShadowmap, vec3(coords.xy, cascadeIndex)).r; 
	float bias = max(0.05 * (1.0 - dot(fNormal, -DirectionalLights[0].Direction)), 0.005); 
	
    float shadow = 1.0;
	
	if (compare - bias > closestDepth)
	{
		shadow = 0.0;
	}

	return shadow;
}

float GetAverageOccluderDepth(float searchAreaSize, int shadowMapSize, vec4 shadowMapCoord, float cascadeIndex)
{
    // calculate the mip level corresponding to the search area
    // Really, mipLevel would be a passed in as a constant.
    float mipLevel = log2(searchAreaSize * shadowMapSize);

    // retrieve the distribution's moments for the entire area
    // shadowMapSampler is a trilinear sampler, not a comparison sampler
    vec2 moments = textureLod(gShadowmap, vec3(shadowMapCoord.xy, cascadeIndex), mipLevel).xy;

    float averageTotalDepth = moments.x;        // assign for semantic clarity
    float probability = VarianceShadow(moments, shadowMapCoord.z);   
   
    // prevent numerical issues
    if (probability > .99) return 0.0;

    // calculate the average occluder depth
    return (averageTotalDepth - probability * shadowMapCoord.z) / (1.0 - probability);
}


float EstimatePenumbraSize(float lightSize, int shadowMapSize, vec4 shadowMapCoord, float penumbraScale, float cascadeIndex)
{
    // the search area covers twice the light size
    float averageOccluderDepth = GetAverageOccluderDepth(lightSize, shadowMapSize, shadowMapCoord, cascadeIndex);
    float penumbraSize = lightSize * (shadowMapCoord.z - averageOccluderDepth) * penumbraScale;

    // clamp to the maximum softness, which matches the search area
    return min(penumbraSize, lightSize);
}



float ShadowCalculation2(vec4 fragPosLightSpace, float cascadeIndex)
{

	/* Constants */

	float lightSize = 1.0f;
	float penumbraScale = 1.0f;
	int shadowMapSize = 4096;
	float poissonRadius = 1.0f;
	int numShadowSamples = 5;


	vec4 shadowMapCoord = fragPosLightSpace / fragPosLightSpace.w;
    shadowMapCoord = shadowMapCoord * 0.5 + 0.5;


	float penumbraSize = EstimatePenumbraSize(lightSize, shadowMapSize, fragPosLightSpace, penumbraScale, cascadeIndex);

	vec2 moments = vec2(0.0);
	// ditherTexture contains 2d rotation matrix (cos, -sin, sin, cos), this will tile the texture across the screen
	/*vec4 rotation = vec4()ditherTexture.SampleLevel(nearestWrapSampler, screenUV * screenSize / ditherTextureSize, 0) * 2.0 - 1.0;

	// calculate the mip level for the disk sample's area
	// Sample points are expected to be penumbraSize * poissonRadius * shadowMapSize texels apart
	// poissonRadius is half the minimum distance in the disk distribution
	float mipLevel = log2(penumbraSize * poissonRadius * shadowMapSize);

	for (int i = 0; i < numShadowSamples; ++i) 
	{
		// poissonDiskValues contain the sampling offsets in the unit circle
		// scale by penumbraSize / 2 to get samples within the penumbra radius (penumbraSize is diameter)
		vec2 sampleOffset = poissonDiskValues[i] * penumbraSize / 2;
		vec4 coord = shadowMapCoord;

		// add rotated sample offset using dithered sample
		coord.x += sampleOffset.x * rotation.x + sampleOffset.y * rotation.y;
		coord.y += sampleOffset.x * rotation.z + sampleOffset.y * rotation.w;

		// shadowMapSampler is a trilinear sampler, not a comparison sampler
		moments += textureLod(gShadowmap, vec3(shadowMapCoord.xy, cascadeIndex), mipLevel).xy;
	}*/
	moments /= numShadowSamples;

	return VarianceShadow(moments, shadowMapCoord.z);
}

float ShadowCalculation(vec4 fragPosLightSpace, float cascadeIndex)
{

    vec4 shadowMapCoord = fragPosLightSpace / fragPosLightSpace.w;
    shadowMapCoord = shadowMapCoord * 0.5 + 0.5;

	float depthCompare = shadowMapCoord.z;

	vec2 moments = texture(gShadowmap, vec3(shadowMapCoord.xy, cascadeIndex)).xy;


    return VarianceShadow(moments, depthCompare);
} 




/* ------------------ Light func -----------------*/


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
	vec3 lightPos = (View * vec4(light.Position,1)).xyz;


    vec3 lightDir = normalize(lightPos - fragPos);

    // attenuation
    float dist    = length(lightPos - fragPos);

	float denom = (dist / light.Radius + 1);
    float attenuation = light.Intensity / (denom * denom);    

	vec3 diffuseLight = light.Color.rgb * attenuation * max(dot(lightDir, normal), 0.0);
	

	vec3 specularLight = vec3(0);

	vec3 toLightDir = lightPos - fragPos;
	vec3 viewDir = normalize(-fragPos);
	vec3 halfwayDir = normalize(toLightDir + viewDir);  


	specularLight += light.Color.rgb * pow(max(dot(normal, halfwayDir), 0.0), 32) * attenuation;

    
    return specularLight + diffuseLight;
} 

void main()
{             
    // retrieve data from gbuffer
    vec4 FragPos = vec4(UnpackPosition(fTexCoord), 1.0);

    vec3 Normal =  UnpackNormal(fTexCoord);
	vec4 fullAlbedo = texture(gAlbedo, fTexCoord);

    vec3 Albedo = fullAlbedo.rgb;
	float AmbientOcclusion = texture(gHbao, fTexCoord).r;


    // then calculate lighting as usual
    vec3 ambient = vec3(0.3f * Albedo * AmbientOcclusion);


	vec3 lightDir = -normalize(mat3(View) * DirectionalLights[0].Direction).rgb;
	float diffFactor = max(dot(lightDir, Normal), 0.0);
	vec3 diffuse = Albedo * diffFactor * AmbientOcclusion;

	// Choose cascade index
	int cascadeIdx = 0; 
	for(int i = 0; i < 5; ++i)
	{
		if(abs(FragPos.z) < DirectionalLights[0].Info[i].Depth)
		{
			cascadeIdx = i;
			break;
		}
	}

	// Compute shadow factor
	vec4 fragPosLightSpace = DirectionalLights[0].Info[cascadeIdx].VPMatrix * InverseView * FragPos;
    float shadow = ShadowCalculation(fragPosLightSpace, cascadeIdx);     


	// Compute point lights

	vec3 lighting = vec3(0);
	for (int i = 0; i < NumLights; i++)
	{
		lighting += CalcPointLight(PointLights[i], Normal, FragPos.xyz);
	}


	// Combine all colors

    FragColor = vec4(ambient + shadow * diffuse + lighting, fullAlbedo.a);
}


