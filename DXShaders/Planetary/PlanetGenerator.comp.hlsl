
#include "Perlin.hlsl"

struct Vertex
{
    float3 Position;
    float3 Normal;
    float3 Tangent;
    float3 Bitangent;
    float2 TexCoord;
};

struct Crater
{
    float3 Center;
    float radius;
    float depth;
    float rimWidth;
    float rimHeight;
};

struct QuantizedShading
{
    int3 Normal;
    int3 Tangent;
    int3 Bitangent;
};

// UAV for compute writes
RWStructuredBuffer<Vertex> vertices : register(u0);
RWStructuredBuffer<Crater> craters : register(u1);
RWStructuredBuffer<QuantizedShading> QuantizedNormals : register(u2);


struct NoiseParams
{
    float frequency;
    float octaves;
    float scaleWidth;
    float scaleHeight;
    float persistence;
    float lacunarity;
};

cbuffer OceanNoise : register(b0)
{
    NoiseParams OceanNoise;
};

cbuffer ContinentNoise : register(b1)
{
    NoiseParams ContinentNoise;
};

cbuffer MountainMaskNoise : register(b2)
{
    NoiseParams MountainMaskNoise;
};

cbuffer CraterParams : register(b3)
{
    int NumCraters;
}

float3 ToSphericalCoords(float3 position)
{
    float r = length(position);
    return float3(r, atan2(position.y, position.x), acos(position.z / r));
}

float3 ToCartesianCoords(float3 angles)
{
    float r = angles.x;
    float theta = angles.y;
    float phi = angles.z;
    
    return float3(r * sin(phi) * cos(theta), r * sin(phi) * sin(theta), r * cos(phi));
}

float smin(float a, float b, float k)
{
    float r = exp(-a / k) + exp(-b / k);
    return -k * log(r);
}

float CraterHeight(float x, float radius, float depth, float rimHeight, float rimWidth)
{
    float inner = x * x * (rimHeight + depth) / (radius * radius) - depth;
    
    float g = x - radius - rimWidth;
    float outer = g * g * rimHeight / (rimWidth * rimWidth);

    return min(inner, outer);
}


float DetailHeight(float3 pos, NoiseParams params)
{
    float total = 0.0;
    float maxAmplitude = 0.0;
    float amplitude = 1.0f;
    float frequency = params.frequency;
    
    for (int i = 0; i < params.octaves; i++)
    {
        total += amplitude * perlin3D(pos * (params.scaleWidth * frequency));
        maxAmplitude += amplitude;

        amplitude *= params.persistence;
        frequency *= params.lacunarity;
    }
    
    return params.scaleHeight * total / maxAmplitude;
}

float DetailHeightDistorted(float3 pos, NoiseParams params)
{
    float3 p = float3(
            DetailHeight(pos, params),
            DetailHeight(pos + float3(1,2,1), params),
            DetailHeight(pos + float3(-4,3, 0), params)
        );
    
    return DetailHeight(pos + 4 * p, params);
}


float Craters(float3 pos)
{
    if (NumCraters <= 0)
        return 0;
    
    int numCraters = min(NumCraters, 3000);
    float total = 0;
    for (int i = 0; i < numCraters; i++)
    {
        float radius = craters[i].radius;
        float depth = craters[i].depth;
        float rimHeight = craters[i].rimHeight;
        float rimWidth = craters[i].rimWidth;

        // pre-normalize crater centers on CPU so this line is gone:
        float3 CraterCenter = craters[i].Center;

        // use dot product for angular distance
        float dotp = dot(CraterCenter, pos); // both are unit vectors
        // acos(dot) gives angular distance in radians
        // but we can compare with cos directly for culling
        float cosMaxAngle = cos(radius + rimWidth);

        // early skip if outside influence
        if (dotp < cosMaxAngle)
            continue;

        // now compute actual angular distance (cheap since few craters pass)
        float dst = acos(dotp);

        total += CraterHeight(dst, radius, depth, rimHeight, rimWidth);
    }
    return total;
}

float SimpleTerrain(float3 pos)
{
    float oceanFloorDepth = 0.1f;
    float continentShape = DetailHeightDistorted(pos, ContinentNoise);
    continentShape *= (continentShape < 0) ? 1 + 2.5f : 1;
    
    //float mountainMask = lerp(0, DetailHeight(pos, 1, 6, 1.2f, 0.01, 0.4f, 2.0f), 0.5f);
    //float mountainShape = Ridges(pos);// * mountainMask;
    
    return continentShape;// + mountainShape;

}

[numthreads(256, 1, 1)] // 256 threads per group
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint idx = DTid.x;

    uint numVertices;
    uint stride;
    vertices.GetDimensions(numVertices, stride);
    
    if (idx < numVertices)
    { 
        QuantizedNormals[idx].Normal = int3(0, 0, 0);
        QuantizedNormals[idx].Tangent = int3(0, 0, 0);
        QuantizedNormals[idx].Bitangent = int3(0, 0, 0);
        
        float3 pos = normalize(vertices[idx].Position);
        float3 spherical = ToSphericalCoords(pos);
        //spherical.x += Craters(pos);
        //spherical.x += Ridges(pos);
        spherical.x += SimpleTerrain(pos);
        vertices[idx].Position = ToCartesianCoords(spherical);
    }
}