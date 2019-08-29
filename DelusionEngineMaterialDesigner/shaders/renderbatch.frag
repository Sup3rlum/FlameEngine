#version 450

in vec2 fTexcoord;

out vec4 fragColor;

uniform sampler2D Texture;
uniform vec4 Color = vec4(1.0,1.0f,1.0,1.0);

void main()
{
	fragColor = texture(Texture, fTexcoord) * Color;
}