#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 World;
uniform mat4 LightViewProjection;

out vec2 fTexCoord;
out vec3 fNormal;
out vec4 fragPosLightSpace;

void main() 
{
    gl_Position = Projection * View * World	* vec4(position, 1.0f);
	fTexCoord = texCoord;
	fNormal = normal;
	
	vec3 fragPos = vec3(World * vec4(position, 1.0f));
	fragPosLightSpace = LightViewProjection * vec4(fragPos, 1.0f);

} 