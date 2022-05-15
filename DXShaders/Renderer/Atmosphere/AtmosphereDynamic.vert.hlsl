

struct VSInput
{
    float3 Position : POSITION;
    /*float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;*/
};

struct PSInput
{
    float4 Position : SV_Position;
    float3 vDirection : VDIRECTION0;
    float2 TexCoord : TEXCOORD0;
};


cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
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




PSInput main(VSInput input)
{
    PSInput output;
    
    output.Position = float4(input.Position, 1);
    
    float4 ViewPos = mul(InverseProjection, float4(input.Position, 1.0));
    output.vDirection = mul(InverseView, float4(ViewPos.xyz, 0)).xyz;
    
    
    return output;
}