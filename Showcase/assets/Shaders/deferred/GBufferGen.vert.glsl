#version 450


layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 texcoord;


layout(location = 0) out vec2 fTexCoord;
//layout(location = 1) out mat3 TBN;
layout(location = 1) out vec3 fNormal;


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

	//assign texture coords
	fTexCoord = texcoord;


	// Calculate the normal matrix for the vertex 
	mat3 normalMatrix = mat3(View) * mat3(WorldInverseTranspose);

	fNormal = normalMatrix * normal;
	/*vec3 fTangent = normalMatrix * tangent;
	vec3 fBitangent = normalMatrix * bitangent;

	TBN = mat3(fTangent, fBitangent, fNormal);*/


	//Write to position

	gl_Position = Projection * View * World * vec4(position, 1.0f);
}