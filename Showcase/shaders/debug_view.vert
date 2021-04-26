#version 450

layout(location=0) in vec3 pos;
layout(location=1) in vec3 color;

uniform mat4 View;
uniform mat4 Projection;

out vec3 fColor;

void main()
{
	gl_Position = Projection * View * vec4(pos, 1.0);
	fColor = color;
}