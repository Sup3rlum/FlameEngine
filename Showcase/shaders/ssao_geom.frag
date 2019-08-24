#version 450 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

in vec2 TexCoords;
in vec3 FragPos;
in vec4 FragPosClipspace;
in vec3 Normal;

uniform sampler2D Texture;

float getDepth(vec4 _pos)
{
	float zNear = 0.1;
	float zFar = 1000.0;
	float depth = _pos.z / _pos.w;
	return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}
void main()
{    

    gPosition = FragPos;

    gNormal = (normalize(Normal)+1.0f) / 2.0f;

    gAlbedo = texture(Texture, TexCoords).rgb;
}