#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
layout (location = 4) in vec2 aTexCoords;

out vec4 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out vec3 Tangent;
out mat3 TBN;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 World;


void main()
{
    vec4 viewPos = View * World * vec4(aPos, 1.0);
    FragPos = World * vec4(aPos, 1.0);//viewPos;
    TexCoords = aTexCoords;
    
	mat3 normalMatrix = transpose(inverse(mat3(World)));

    Normal = normalMatrix * aNormal;
    Tangent = normalMatrix * aTangent;
    
    TBN = mat3(Tangent, cross(Tangent, Normal), Normal);

    gl_Position = Projection * viewPos;
}