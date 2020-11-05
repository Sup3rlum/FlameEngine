#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 World;

out vec4 screenSpacePosition;

void main() 
{

    screenSpacePosition = Projection * View * World * vec4(position, 1.0f);

    gl_Position = screenSpacePosition;
}