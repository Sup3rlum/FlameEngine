#version 450

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 gTexCoord[];
in vec3 gNormal[];

out vec2 fTexCoord;
out vec3 fNormal;

void main() 
{
	gl_Position = gl_in[0].gl_Position;
	fTexCoord = gTexCoord[0];
	fNormal = gNormal[0];
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	fTexCoord = gTexCoord[1];
	fNormal = gNormal[1];
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	fTexCoord = gTexCoord[2];
	fNormal = gNormal[2];
	EmitVertex();

	EndPrimitive();
}