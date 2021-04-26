#version 450

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position;


// Uniform parameters

uniform vec3 CameraPos;       // The camera's current position
uniform vec3 LightDirection;        // The direction vector to the light source

// Transform uniforms
uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;


// To Fragment shader
out vec3 v3Direction;

out vec3 fSecondaryColor;
out vec3 fColor;



void main(void)
{
    fColor = vec3(0,0.3f,0.85f) * 0.7f;
    fSecondaryColor = vec3(0.85f, 0.93f, 1.0f);

    v3Direction = position;

    gl_Position =  Projection * View * World * vec4(position, 1);
}