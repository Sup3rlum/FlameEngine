#version 330

in vec2 fTexCoord;
in vec3 fNormal;

out vec4 fragColor;

uniform sampler2D _texture;
uniform vec3 LightDirection;

void main() 
{
	vec3 albedo = texture(_texture, fTexCoord).rgb;
	vec3 ambient = albedo * 0.2;

	float diff = max(dot(normalize(-LightDirection), fNormal), 0.0);

	vec3 diffuse = albedo * diff;

    fragColor = vec4(ambient+diffuse,1.0f);
}