#version 450


layout(location = 0) out vec4 FragColor;

layout(location = 0) in vec2 fTexCoord;

/* --------------- Uniforms --------------------*/

layout(binding=0) uniform sampler2D gDepth;
layout(binding=1) uniform sampler2D gNormal;
layout(binding=2) uniform sampler2D gAlbedo;
layout(binding=3) uniform sampler2D gSpecular;
layout(binding=4) uniform sampler2DArray gShadowmap;
layout(binding=5) uniform sampler2D gHbao;


/*----------------------------------------------*/


struct DirectionalLightCascadeInfo
{
	mat4 View;
	mat4 Projection;
	vec4 Depth;
};

struct PointLight
{
	vec3 Position;
	float Intensity;
	float Radius;
	vec4 Color;
};


/*---------------- Light Data ---------------------*/

/*
uniform DirectionalLight DirectionalLights[5];
uniform PointLight PointLights[16];*/

//uniform int NumLights = 1;


layout(std140, binding = 0) uniform CameraMatrices
{
	mat4 View;
	mat4 Projection;
};

layout(std140, binding = 2) uniform CascadeBuffer
{
	DirectionalLightCascadeInfo DirLightCascades[5];
};


layout(std140, binding = 4) uniform CombineBuffer
{
	mat4 InverseView;
	mat4 InverseProjection;
	vec4 DirLightDirection;
	vec4 DirLightInfo;
};





/*-------------------------------------------------*/



const float MinVariance = 0.000001f;

vec3 DebugColors[5] = vec3[5]
(
	vec3(1.0f, 0.0f, 0.0f),
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f),
	vec3(1.0f, 1.0f, 0.0f),
	vec3(0.0f, 1.0f, 1.0f)
);



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
	float bias = max(0.05 * (1.0 - dot(fNormal, -DirLightDirection.rgb)), 0.005); 
	
    float shadow = 1.0;
	
	if (compare - bias > closestDepth)
	{
		shadow = 0.0;
	}

	return shadow;
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

/*
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
*/
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


	vec3 lightDir = -normalize(mat3(View) * DirLightDirection.rgb).rgb;
	float diffFactor = max(dot(lightDir, Normal), 0.0);
	vec3 diffuse = Albedo * diffFactor * AmbientOcclusion;

	// Choose cascade index
	int cascadeIdx = 0; 
	for(int i = 0; i < 5; ++i)
	{
		if(abs(FragPos.z) < DirLightCascades[i].Depth.r)
		{
			cascadeIdx = i;
			break;
		}
	}

	// Compute shadow factor

	mat4x4 dirLightVp = DirLightCascades[cascadeIdx].Projection * DirLightCascades[cascadeIdx].View;

	vec4 fragPosLightSpace = dirLightVp * InverseView * FragPos;
	float shadow = ShadowCalculation(fragPosLightSpace, cascadeIdx);


	// Compute point lights

	vec3 lighting = vec3(0);// DebugColors[cascadeIdx];
	/*
	for (int i = 0; i < NumLights; i++)
	{
		lighting += CalcPointLight(PointLights[i], Normal, FragPos.xyz);
	}*/


	// Combine all colors

    FragColor = vec4(ambient + shadow * diffuse + lighting, fullAlbedo.a);
}


