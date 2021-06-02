#version 450


layout(location=0) in vec3 position;
layout(location=1) in vec2 texcoord;


out vec2 fTexcoord;

uniform mat4x4 View;
uniform mat4x4 MatrixTransforms;


void main()
{
	gl_Position = View * MatrixTransforms * vec4(position, 1.0f);
	fTexcoord = texcoord;
}