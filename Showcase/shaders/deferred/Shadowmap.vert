#version 450

layout(location = 0) in vec3 position;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 World;

out vec4 FragPosLightSpace;

void main()
{
	FragPosLightSpace = View * World * vec4(position, 1.0f);

	gl_Position = Projection * FragPosLightSpace;
}