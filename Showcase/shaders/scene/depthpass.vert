#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
layout (location = 4) in vec2 aTexCoords;


uniform mat4 View;
uniform mat4 Projection;
uniform mat4 World;

void main()
{

    gl_Position = Projection * View * World * vec4(aPos, 1.0);
}