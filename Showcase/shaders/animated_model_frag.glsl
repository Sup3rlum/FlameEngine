#version 410

in vec2 fTexCoord;
in vec3 fNormal;
in vec4 fragPosLightSpace;


out vec4 fragColor;

struct DirectionalLight
{
	vec3 Direction;
	vec4 Color;
	float Intensity;
};


uniform sampler2D _texture;
uniform sampler2D _shadowMap;
uniform DirectionalLight DirectionalLights[3];

/*
uniform float zNear = 0.1f;
uniform float zFar = 100.0f;*/



float VarianceShadow(sampler2D shadowMap, vec2 coords, float compare)
{
	vec2 moments = texture2D(shadowMap, coords.xy).xy;

	float p = step(compare, moments.x);
	float variance = max(moments.y - moments.x * moments.x, 0.00002);

	float d = compare - moments.x;
	float pMax = variance / (variance + d * d);


	return 1.0 - min(max(p, pMax), 1.0);
}

float SimpleShadow(sampler2D shadowMap, vec2 coords, float compare)
{
    float closestDepth = texture(_shadowMap, coords.xy).r; 
	float bias = max(0.05 * (1.0 - dot(fNormal, -DirectionalLights[0].Direction)), 0.005); 
	
    float shadow = 0.0;
	
	if (compare - bias > closestDepth)
	{
		shadow = 1.0;
	}

	return shadow;
}
float SimpleShadowLinear(sampler2D shadowMap, vec2 coords, float compare)
{

	vec2 texelSize = 1.0 / textureSize(_shadowMap, 0);

	vec2 pixelPos = coords / texelSize;
	vec2 fracPart = fract(pixelPos);
	vec2 startTexel = (pixelPos - fracPart) * texelSize;


	float blTexel = SimpleShadow(shadowMap, startTexel, compare);
	float brTexel = SimpleShadow(shadowMap, startTexel + vec2(texelSize.x, 0.0), compare);
	float tlTexel = SimpleShadow(shadowMap, startTexel + vec2(0.0, texelSize.y), compare);
	float trTexel = SimpleShadow(shadowMap, startTexel + texelSize, compare);


	float mixA = mix(blTexel, tlTexel, fracPart.y);
	float mixB = mix(brTexel, trTexel, fracPart.y);

	return mix(mixA, mixB, fracPart.x);
}


float PcfShadow(sampler2D shadowMap, vec2 coords, float compare)
{
    float shadow = 0.0;
	
	vec2 texelSize = 1.0 / textureSize(_shadowMap, 0);

	const float NUM_SAMPLES = 9.0f;
	const float SAMPLES_START = (NUM_SAMPLES  - 1.0f) / 2.0f;

	for(float x = -SAMPLES_START; x <= SAMPLES_START; ++x)
	{
		for(float y = -SAMPLES_START; y <= SAMPLES_START; ++y)
		{

			vec2 coordsOffset = vec2(x,y) * texelSize;
			shadow += SimpleShadowLinear(shadowMap, coords + coordsOffset, compare);   
		}    
	}
	shadow /= (NUM_SAMPLES * NUM_SAMPLES);

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
	vec3 color = texture(_texture, fTexCoord).rgb;
    vec3 normal = normalize(fNormal);
    vec3 lightColor = DirectionalLights[0].Color.rgb;
	
    // ambient
    vec3 ambient = 0.15 * color;
	
    // diffuse
    float diff = max(dot(-DirectionalLights[0].Direction, normal), 0.0);
    vec3 diffuse = diff * lightColor;

	
    // calculate shadow
    float shadow = ShadowCalculation(fragPosLightSpace);       
    vec3 lighting = (ambient + (1.0 - shadow) * diffuse) * color;    
    
    fragColor = vec4(lighting, 1.0);
}