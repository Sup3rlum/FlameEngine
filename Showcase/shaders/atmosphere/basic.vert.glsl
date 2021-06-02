#version 450

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position;


// Uniform parameters


layout(std140, binding=6) uniform CameraData 
{
    uniform vec3 CameraPos;       // The camera's current position
    uniform vec3 LightDirection;        // The direction vector to the light source
};

layout(std140, binding=0) uniform CameraMatrices
{
    mat4 View;
    mat4 Projection;
};

layout(std140, binding=1) uniform TransformBuffer
{
    mat4 World;
};


// To Fragment shader
layout(location = 0) out vec3 v3Direction;
layout(location = 1) out vec3 fSecondaryColor;
layout(location = 2) out vec3 fColor;



void main(void)
{
    fColor = vec3(0,0.3f,0.85f) * 0.7f;
    fSecondaryColor = vec3(0.85f, 0.93f, 1.0f);

    v3Direction = position;

    gl_Position =  Projection * View * World * vec4(position, 1);
}