#version 450


layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

out vec2 fTexCoord;


uniform mat4 Projection;
uniform mat4 Transform;

void main()
{
	gl_Position = Projection * Transform * vec4(position, 1.0f);
	fTexCoord = texcoord;
}