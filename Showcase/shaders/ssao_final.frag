#version 450

out vec4 FragColor;

in vec2 fTexcoord;

uniform sampler2D gDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;
uniform sampler2D _shadowMap;

uniform mat4x4 InverseProjection;
uniform mat4x4 InverseView;
uniform mat4x4 LightViewProjection;

struct DirectionalLight
{
	vec3 Direction;
	vec4 Color;
	float Intensity;
};


uniform DirectionalLight DirectionalLights[3];



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


float VarianceShadow(sampler2D shadowMap, vec2 coords, float compare)
{
	if (coords.x < 0.05 || coords.x > 0.95 || coords.y < 0.05 || coords.y > 0.95)
	{
		return 0.0;
	}


	vec2 moments = texture2D(shadowMap, coords.xy).xy;

	float p = step(compare, moments.x);
	float variance = max(moments.y - moments.x * moments.x, 0.00002);

	float d = compare - moments.x;
	float pMax = smoothstep(0.2, 1.0, variance / (variance + d * d));


	return 1.0 - min(max(p, pMax), 1.0);
}
float SimpleShadow(sampler2D shadowMap, vec2 coords, float compare)
{
	vec3 fNormal =  texture(gNormal, fTexcoord).rgb * 2.0 - 1.0;

    float closestDepth = texture(_shadowMap, coords.xy).r; 
	float bias = max(0.05 * (1.0 - dot(fNormal, -DirectionalLights[0].Direction)), 0.005); 
	
    float shadow = 0.0;
	
	if (compare - bias > closestDepth)
	{
		shadow = 1.0;
	}

	return shadow;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	
    projCoords = projCoords * 0.5 + 0.5;

	
    float currentDepth = projCoords.z;
	

    return VarianceShadow(_shadowMap, projCoords.xy, currentDepth);
}  



void main()
{             
    // retrieve data from gbuffer
    //vec4 FragPos = texture(gPosition, fTexcoord);
    vec4 FragPos = vec4(UnpackPosition(fTexcoord), 1.0);
    vec3 Normal =  UnpackNormal(fTexcoord);
    vec3 Albedo = texture(gAlbedo, fTexcoord).rgb;


	vec4 fragPosLightSpace = LightViewProjection * InverseView * FragPos;


	float AmbientOcclusion = texture(ssao, fTexcoord).r;
    
    // then calculate lighting as usual
    vec3 ambient = vec3(0.4f * Albedo * AmbientOcclusion);


	vec3 lightDir = -normalize(DirectionalLights[0].Direction).rgb;

	float diffFactor = max(dot(lightDir, Normal), 0.0);
	vec3 diffuse = Albedo * diffFactor * AmbientOcclusion;

    float shadow = ShadowCalculation(fragPosLightSpace);     

    FragColor = vec4(ambient + (1.0 - shadow) * diffuse, 1.0);


}