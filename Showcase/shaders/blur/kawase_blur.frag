#version 450

in vec2 fTexcoord;

out vec4 fragColor;

uniform sampler2D Texture;
uniform vec4 Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

uniform float PassIndex;

void main()
{

	vec2 resolution = textureSize(Texture, 0);
	
	vec3 color = texture(Texture, fTexcoord + vec2(PassIndex, PassIndex) / resolution).rgb;
	color += texture(Texture, fTexcoord + vec2(-PassIndex, PassIndex) / resolution).rgb;
	color += texture(Texture, fTexcoord + vec2(PassIndex, -PassIndex) / resolution).rgb;
	color += texture(Texture, fTexcoord + vec2(-PassIndex, -PassIndex) / resolution).rgb;

	color /= 4.0;

	
	fragColor = vec4(color, 1.0);
}