
struct Vertex
{
    float3 Position;
    float3 Normal;
    float3 Tangent;
    float3 Bitangent;
    float2 TexCoord;
};

struct QuantizedShading
{
    int3 Normal;
    int3 Tangent;
    int3 Bitangent;
};


// UAVs for compute access
RWStructuredBuffer<Vertex> vertices : register(u0);
RWStructuredBuffer<QuantizedShading> quantizedShading : register(u1);
RWStructuredBuffer<uint> indices : register(u2);

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint triIdx = DTid.x; // one thread per triangle

    uint numIndices, stride;
    indices.GetDimensions(numIndices, stride);

    uint numTriangles = numIndices / 3;
    if (triIdx >= numTriangles)
        return;

    // Fetch indices of this triangle
    uint i0 = indices[triIdx * 3 + 0];
    uint i1 = indices[triIdx * 3 + 1];
    uint i2 = indices[triIdx * 3 + 2];

    // Fetch vertex positions
    float3 p0 = vertices[i0].Position;
    float3 p1 = vertices[i1].Position;
    float3 p2 = vertices[i2].Position;
    
    // Fetch vertex UV space
    float2 uv0 = vertices[i0].TexCoord;
    float2 uv1 = vertices[i1].TexCoord;
    float2 uv2 = vertices[i2].TexCoord;

    // Compute face normal
    float3 e1 = p1 - p0;
    float3 e2 = p2 - p0;
    float3 faceNormal = normalize(cross(e1, e2));
    
    // Tangent / bitangent
    float2 dUV1 = uv1 - uv0;
    float2 dUV2 = uv2 - uv0;
    float det = dUV1.x * dUV2.y - dUV2.x * dUV1.y;
    float f = (abs(det) < 1e-8) ? 0.0 : 1.0 / det;

    float3 tangent = f * (dUV2.y * e1 - dUV1.y * e2);
    float3 bitangent = f * (-dUV2.x * e1 + dUV1.x * e2);

    float QUANTIIZE_FACTOR = 1024.0;
    int3 qN = int3(faceNormal * QUANTIIZE_FACTOR);
    int3 qT = int3(tangent * QUANTIIZE_FACTOR);
    int3 qB = int3(bitangent * QUANTIIZE_FACTOR);
    
    // Atomic accumulation for each vertex
    uint idxs[3] = { i0, i1, i2 };
    [unroll]
    for (int v = 0; v < 3; v++)
    {
        uint idx = idxs[v];

        InterlockedAdd(quantizedShading[idx].Normal.x, qN.x);
        InterlockedAdd(quantizedShading[idx].Normal.y, qN.y);
        InterlockedAdd(quantizedShading[idx].Normal.z, qN.z);

        InterlockedAdd(quantizedShading[idx].Tangent.x, qT.x);
        InterlockedAdd(quantizedShading[idx].Tangent.y, qT.y);
        InterlockedAdd(quantizedShading[idx].Tangent.z, qT.z);

        InterlockedAdd(quantizedShading[idx].Bitangent.x, qB.x);
        InterlockedAdd(quantizedShading[idx].Bitangent.y, qB.y);
        InterlockedAdd(quantizedShading[idx].Bitangent.z, qB.z);
    }
}