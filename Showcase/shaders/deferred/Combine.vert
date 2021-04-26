#version 420



layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;


out vec2 fTexCoord;

void main()
{
	gl_Position = vec4(position, 1.0f);
	fTexCoord = texcoord;
}