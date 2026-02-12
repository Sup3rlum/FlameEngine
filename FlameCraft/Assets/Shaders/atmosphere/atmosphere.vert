#version 450

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position;


// Uniform parameters

uniform vec3 CameraPos;       // The camera's current position
uniform vec3 LightDirection;        // The direction vector to the light source

uniform vec3 InvWavelength = vec3(    // 1 / pow(wavelength, 4) for the red, green, and blue channels
    1.0f / pow(0.650f, 4.0f), // Red wavelength = 650 nm
    1.0f / pow(0.570f, 4.0f), // Green wavelength = 570 nm
    1.0f / pow(0.475f, 4.0f)); // Blue wavelength = 475 nm


uniform float fOuterRadius = 11.0f;                     // The outer (atmosphere) radius
uniform float fInnerRadius = 10.0f;                     // The inner (planetary) radius
uniform float fKrESun = 0.0025f * 20.0f;                // Kr * ESun
uniform float fKmESun = 0.0015f * 20.0f;                // Km * ESun
uniform float fKr4PI = 0.0025f * 4.0f * 3.141592653f;   // Kr * 4 * PI
uniform float fKm4PI = 0.0015f * 4.0f * 3.141592653f;   // Km * 4 * PI
uniform float fScale = 1.0f;                            // 1 / (fOuterRadius - fInnerRadius)
uniform float fScaleDepth = 0.25f;                              // The scale depth (i.e. the altitude at which the atmosphere's average density is found)
uniform float fScaleOverScaleDepth = 4.0f;                     // fScale / fScaleDepth


// Transform uniforms
uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;

const int nSamples = 2;


// To Fragment shader
out vec3 v3Direction;

out vec3 fSecondaryColor;
out vec3 fColor;


float scale(float fCos)
{
    float x = 1.0 - fCos;
    return fScaleDepth * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.80 + x * 5.25))));
}


void main(void)
{

// Get the ray from the camera to the vertex, and its length (which is the far point of the ray passing through the atmosphere)
    vec3 v3Ray = position;
    float fFar = length(v3Ray);
    v3Ray /= fFar;

    // Calculate the ray's starting position, then calculate its scattering offset
    vec3 v3Start = CameraPos;
    float fCameraHeight = CameraPos.y;
    float fHeight = length(v3Start);
    float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
    float fStartAngle = dot(v3Ray, v3Start) / fHeight;
    float fStartOffset = fDepth * scale(fStartAngle);

    // Initialize the scattering loop variables

    float fSampleLength = fFar / float(nSamples);
    float fScaledLength = fSampleLength * fScale;
    vec3 v3SampleRay = v3Ray * fSampleLength;
    vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

    // Now loop through the sample rays
    vec3 v3FrontColor = vec3(0.2, 0.1, 0.0);
    for (int i = 0; i < nSamples; i++)
    {
        float fHeight = length(v3SamplePoint);
        float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
        float fLightAngle = dot(-LightDirection, v3SamplePoint) / fHeight;
        float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
        float fScatter = (fStartOffset + fDepth * (scale(fLightAngle) - scale(fCameraAngle)));
        vec3 v3Attenuate = exp(-fScatter * (InvWavelength * fKr4PI + fKm4PI));
        v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
        v3SamplePoint += v3SampleRay;
    }

    // Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
    fSecondaryColor = v3FrontColor * fKmESun;
    fColor = v3FrontColor * (InvWavelength * fKrESun);



    v3Direction = normalize(-position);

    gl_Position =  Projection * View * World * vec4(position, 1);
}