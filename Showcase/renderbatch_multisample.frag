#version 450

in vec2 fTexcoord;

out vec4 fragColor;

uniform sampler2DMS Texture;
uniform vec4 Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

void main()
{
	fragColor = texelFetch(Texture, ivec2(fTexcoord * vec2(2560,1440)), 0) * Color;


}