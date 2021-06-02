#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

in float gData[];

out vec2 fTexCoord;
out vec3 fNormal;
out vec3 fColor;

uniform mat4x4 View;
uniform mat4x4 Projection;
uniform mat4x4 World;


vec3 vertices[24] = vec3[]
(

	// Z		0
	vec3(0.0f,	0.0f,	0.0f),
	vec3(1.0f,	0.0f,	0.0f),
	vec3(0.0f,	1.0f,	0.0f),
	vec3(1.0f,	1.0f,	0.0f),

	// Z''
	vec3(0.0f,	0.0f,	1.0f),
	vec3(1.0f,	0.0f,	1.0f),
	vec3(0.0f,	1.0f,	1.0f),
	vec3(1.0f,	1.0f,	1.0f),

		// Y 
	vec3(0.0f,	0.0f,	0.0f),
	vec3(1.0f,	0.0f,	0.0f),
	vec3(0.0f,	0.0f,	1.0f),
	vec3(1.0f,	0.0f,	1.0f),

		// Y' 
	vec3(0.0f,	1.0f,	0.0f),
	vec3(1.0f,	1.0f,	0.0f),
	vec3(0.0f,	1.0f,	1.0f),
	vec3(1.0f,	1.0f,	1.0f),

		// Z 
	vec3(0.0f,  0.0f,	0.0f),
	vec3(0.0f,  1.0f,	0.0f),
	vec3(0.0f,  0.0f,	1.0f),
	vec3(0.0f,  1.0f,	1.0f),

		// Z 
	vec3(1.0f,  0.0f,	0.0f),
	vec3(1.0f,  1.0f,	0.0f),
	vec3(1.0f,  0.0f,	1.0f),
	vec3(1.0f,  1.0f,	1.0f)
);

vec3 normals[6] = vec3[]
(
	vec3(0, 0, -1),
	vec3(0, 0, 1),
	vec3(0, -1, 0),
	vec3(0, 1, 0),
	vec3(-1, 0, 0),
	vec3(1, 0, 0)
);

vec2 uvs[4] = vec2[]
(
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(0.0f, 0.0f),
	vec2(0.0f, 1.0f)
);			



void main() 
{
	for (int i = 0; i < 4; i++)
	{
		gl_Position = Projection * View * World * (gl_in[0].gl_Position + vec4(vertices[i+int(gData[0])*4],0));
		fTexCoord = uvs[i];
		fNormal = normals[int(gData[0])];
		fColor = vec3(gData[0]) / 6.0;
		EmitVertex();
	}

	EndPrimitive();
}