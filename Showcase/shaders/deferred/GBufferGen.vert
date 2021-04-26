#version 450


layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 texcoord;


out vec2 fTexCoord;
out mat3 TBN;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 World;


void main()
{

	//assign texture coords
	fTexCoord = texcoord;


	// Calculate the normal matrix for the vertex 
	mat3 normalMatrix = mat3(View) * transpose(inverse(mat3(World)));

	vec3 fNormal = normalMatrix * normal;
	vec3 fTangent = normalMatrix * tangent;
	vec3 fBitangent = normalMatrix * bitangent;

	TBN = mat3(fTangent, fBitangent, fNormal);


	//Write to position

	gl_Position = Projection * View * World * vec4(position, 1.0f);
}