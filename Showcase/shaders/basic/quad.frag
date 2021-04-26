#version 450

in vec2 fTexCoord;

layout(binding=0) uniform sampler2D Texture;

out vec4 fragColor;

void main()
{
	fragColor = texture(Texture, fTexCoord);
}