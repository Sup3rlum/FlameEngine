#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec4 FragPos;
out vec4 FragPosWorld;
out vec2 TexCoords;
out vec3 Normal;


uniform mat4 View;
uniform mat4 Projection;
uniform mat4 World;


void main()
{
    vec4 viewPos = View * World * vec4(aPos, 1.0);
    FragPos = viewPos;
    FragPosWorld = World * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
    
	mat3 normalMatrix = transpose(inverse(mat3(World)));

    Normal = normalMatrix * aNormal;
    
    gl_Position = Projection * viewPos;
}