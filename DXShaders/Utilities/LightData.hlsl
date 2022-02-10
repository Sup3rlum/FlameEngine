struct CascadeInfo
{
    matrix LView;
    matrix LProjection;
    float4 MaxDepth;
};


struct DirectionalLight
{
    float4 Direction;
    float4 Radiance;
};

struct PointLight
{
    float4 Position;
    float4 Radiance;
    float Radius;
};

struct SpotLight
{
    float4 Position;
    float4 Direction;
    float4 Radiance;
    float Radius;
    float ApertureSize;
    float ApertureSharpness;
};
