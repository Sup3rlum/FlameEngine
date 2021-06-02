
#version 450

layout(location = 0) in vec3 position;


layout(std140, binding = 0) uniform CameraMatrices
{
	mat4 View;
	mat4 Projection;
};

layout(std140, binding = 1) uniform TransformBuffer
{
	mat4 World;
	mat4 WorldInverseTranspose;
};


void main()
{
	gl_Position = Projection * View * World * vec4(position, 1.0f);
}