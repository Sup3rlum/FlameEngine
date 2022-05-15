#include "Scattering.hlsl"


struct PSInput
{
    float4 Position : SV_Position;
    float3 vDirection : VDIRECTION0;
};


cbuffer AtmosphereConstantBuffer : register(b9)
{
    matrix InverseProjection;
    matrix InverseView;
    
    float4 LightDirection;
    
    float3 RayleighScatteringCoeff;
    float MieScatteringCoeff;
    
    float RayleighScale;
    float MieScale;
    float MieDir;
    float SunIntensity;
    
    float PlanetRadius;
    float AtmosphereThickness;
    int PrimarySteps;
    int SecondarySteps;
};


float4 main(PSInput input) : SV_TARGET
{
    float3 CameraPosition = float3(0, PlanetRadius + 1100.0f, 0);
    float AtmosphereRadius = PlanetRadius + AtmosphereThickness;
    
    float3 FinalColor = AtmosphericScattering(
        normalize(input.vDirection), // normalized ray direction
        float3(0, 6372e3, 0), // ray origin
        normalize(-LightDirection.rgb), // position of the sun
        SunIntensity, // intensity of the sun
        PlanetRadius, // radius of the planet in meters
        6471e3, // radius of the atmosphere in meters
        RayleighScatteringCoeff, // Rayleigh scattering coefficient
        MieScatteringCoeff, // Mie scattering coefficient
        RayleighScale, // Rayleigh scale height
        MieScale, // Mie scale height
        MieDir, // Mie preferred scattering direction
        32, // Primary Ray steps
        8 // Secondary ray steps
    );
    
    return float4(FinalColor, 1.0f);
}

