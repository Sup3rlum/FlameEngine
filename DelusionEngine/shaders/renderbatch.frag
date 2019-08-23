#version 450

in vec2 fTexcoord;

out vec4 fragColor;

uniform sampler2D Texture;

void main()
{
	fragColor = texture(Texture, fTexcoord);
}