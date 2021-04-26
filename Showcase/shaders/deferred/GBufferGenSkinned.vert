#version 450


const int MAX_JOINTS = 32;
const int MAX_WEIGHTS = 4;


layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 texcoord;

layout(location = 5) in ivec4 jointIndices;	// should be OK
layout(location = 6) in vec4 jointWeights;  // OK

out vec2 fTexCoord;
out mat3 TBN;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 World;


uniform mat4 jointTransforms[MAX_JOINTS];



void main()
{
	vec4 totalLocalPos = vec4(0.0);
	vec4 totalNormal = vec4(0.0);
	vec4 totalTangent = vec4(0.0);
	vec4 totalBitangent = vec4(0.0);
	

	float totalWeight = 0;

	for(int i=0;i<MAX_WEIGHTS;i++)
	{
		mat4 jointTransform = jointWeights[i] * jointTransforms[jointIndices[i]];

		totalLocalPos	+= jointTransform * vec4(position, 1.0);
		totalNormal		+= jointTransform * vec4(normal, 0.0);
		totalTangent	+= jointTransform * vec4(tangent, 0.0);
		totalBitangent	+= jointTransform * vec4(bitangent, 0.0);

		totalWeight += jointWeights[i];
	}


	//assign texture coords
	fTexCoord = texcoord;


	// Calculate the normal matrix for the vertex 
	mat3 normalMatrix = mat3(View) * transpose(inverse(mat3(World)));
	vec3 fNormal = normalMatrix * totalNormal.xyz;
	vec3 fTangent = normalMatrix * totalTangent.xyz;
	vec3 fBitangent = normalMatrix * totalBitangent.xyz;

	TBN = mat3(fTangent, fBitangent, fNormal);


	//Write to position

	//gl_Position = Projection * View * World * vec4(totalLocalPos.xyz, 1);
	gl_Position = Projection * View * World * totalLocalPos;
}