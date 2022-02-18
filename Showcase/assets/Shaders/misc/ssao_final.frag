#version 450

out vec4 FragColor;




in vec2 fTexcoord;




uniform sampler2D gDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;
uniform sampler2D ssao;
uniform sampler2D _shadowMap;

uniform mat4x4 InverseProjection;
uniform mat4x4 InverseView;

struct DirectionalLight
{
	vec3 Direction;
	vec4 Color;
	float Intensity;
	mat4x4 VPMatrix;
};
struct PointLight
{
	vec3 Position;
	float Intensity;
	float Radius;
	vec4 Color;
};


uniform DirectionalLight DirectionalLights[5];
uniform PointLight PointLights[16];

uniform int NumLights;
uniform vec3 CameraPosition;


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

	vec2 moments = texture2D(shadowMap, coords.xy).xy;

	float p = step(compare, moments.x);
	float variance = max(moments.y - moments.x * moments.x, 0.0);

	float d = compare - moments.x;
	float pMax = smoothstep(0.0, 1.0, variance / (variance + d * d));


	return min(max(p, pMax), 1.0);
}
float PcfShadow(sampler2D shadowMap, vec2 coords, float compare)
{
	vec3 fNormal =  texture(gNormal, fTexcoord).rgb * 2.0 - 1.0;

    float closestDepth = texture(_shadowMap, coords.xy).r; 
	float bias = max(0.05 * (1.0 - dot(fNormal, -DirectionalLights[0].Direction)), 0.005); 
	
    float shadow = 1.0;
	
	if (compare - bias > closestDepth)
	{
		shadow = 0.0;
	}

	return shadow;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	
    projCoords = projCoords * 0.5 + 0.5;

    return VarianceShadow(_shadowMap, projCoords.xy, projCoords.z);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
    vec3 lightDir = normalize(light.Position - fragPos);

    // attenuation
    float dist    = length(light.Position - fragPos);

	float denom = (dist / light.Radius + 1);

    float attenuation = light.Intensity / (denom * denom);    


	vec3 diffuseLight = light.Color.rgb * attenuation * max(dot(lightDir, normal), 0.0);
	

	vec3 specularLight = vec3(0);

	vec3 toLightDir = light.Position - fragPos;
	vec3 viewDir = normalize(CameraPosition - fragPos);
	vec3 halfwayDir = normalize(toLightDir + viewDir);  


   // float glossIndex = texture(gSpecular, fTexcoord).r;


	specularLight += light.Color.rgb * pow(max(dot(normal, halfwayDir), 0.0), 32) * attenuation;

    


    return specularLight + diffuseLight;
	//return light.Color.rgb;
} 

void main()
{             



    // retrieve data from gbuffer
    //vec4 FragPos = texture(gPosition, fTexcoord);
    vec4 FragPos = vec4(UnpackPosition(fTexcoord), 1.0);

    vec2 fragTexCoord = fTexcoord;


    vec3 Normal =  UnpackNormal(fragTexCoord);

	vec4 fullAlbedo = texture(gAlbedo, fragTexCoord);
    vec3 Albedo = fullAlbedo.rgb;


	vec4 fragPosLightSpace = DirectionalLights[0].VPMatrix * InverseView * FragPos;


	float AmbientOcclusion = texture(ssao, fragTexCoord).r;
    // then calculate lighting as usual
    vec3 ambient = vec3(0.2f * Albedo * AmbientOcclusion);


	vec3 lightDir = -normalize(DirectionalLights[0].Direction).rgb;

	float diffFactor = max(dot(lightDir, Normal), 0.0);
	vec3 diffuse = Albedo * diffFactor * AmbientOcclusion * 0.8;

    float shadow =  ShadowCalculation(fragPosLightSpace);     


	vec3 lighting = vec3(0);

	for (int i = 0; i < NumLights; i++)
	{
		lighting += CalcPointLight(PointLights[i], Normal, (InverseView * FragPos).xyz);

	}


    FragColor = vec4(ambient + shadow * diffuse + lighting, fullAlbedo.a);


}