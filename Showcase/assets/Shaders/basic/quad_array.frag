#version 450

in vec2 fTexCoord;

layout(binding=0) uniform sampler2DArray Texture;

out vec4 fragColor;

uniform float Layer = 0;
uniform float MaxLayers = 5;

void main()
{
	fragColor = texture(Texture, vec3(fTexCoord, Layer));
}