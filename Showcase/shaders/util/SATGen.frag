#version 450

out vec4 FragColor;

in vec2 fTexCoord;

float texelSize = 1.0f/4096.0f;

layout(binding=0) uniform sampler2DArray Texture;

uniform float PassIndex;
uniform vec2 SumDirection;
uniform float LayerIndex;


void main()
{

	float phase = pow(4.0f, PassIndex) * texelSize;

	vec2 offset = SumDirection * phase;

	vec4 t0 = texture(Texture, vec3(fTexCoord, LayerIndex));
	vec4 t1 = texture(Texture, vec3(fTexCoord + offset, LayerIndex));
	vec4 t2 = texture(Texture, vec3(fTexCoord + offset * 2, LayerIndex));
	vec4 t3 = texture(Texture, vec3(fTexCoord + offset * 3, LayerIndex));
	vec4 t4 = texture(Texture, vec3(fTexCoord + offset * 4, LayerIndex));


	vec4 finalColor = t0 + t1 + t2 + t3 + t4;

}