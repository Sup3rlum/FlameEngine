#version 450

const int MAX_JOINTS = 32;
const int MAX_WEIGHTS = 4;

layout(location = 0) in vec3 position;
layout(location = 5) in ivec4 jointIndices;	// should be OK
layout(location = 6) in vec4 jointWeights;  // OK


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


layout(std140, binding = 3) uniform JointsBuffer
{
	mat4 jointTransforms[MAX_JOINTS];
};

void main()
{
	vec4 totalLocalPos = vec4(0.0);

	for(int i=0;i<MAX_WEIGHTS;i++)
	{
		mat4 jointTransform = jointWeights[i] * jointTransforms[jointIndices[i]];

		totalLocalPos	+= jointTransform * vec4(position, 1.0);
	}


	gl_Position = Projection * View * World * totalLocalPos;
}

