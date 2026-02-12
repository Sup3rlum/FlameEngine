
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
	uint vertIdx = DTid.x;
	uint numVertices, stride;
	vertices.GetDimensions(numVertices, stride);
	if (vertIdx >= numVertices)
		return;

	const float SCALE = 1e6;

	float3 N = (float3) quantizedShading[vertIdx].Normal / SCALE;
	float3 T = (float3) quantizedShading[vertIdx].Tangent / SCALE;
	float3 B = (float3) quantizedShading[vertIdx].Bitangent / SCALE;

    // Normalize + Gram-Schmidt orthogonalization
	N = normalize(N);
	T = normalize(T - N * dot(N, T));
	B = normalize(cross(N, T));

	vertices[vertIdx].Normal = N;
	vertices[vertIdx].Tangent = T;
	vertices[vertIdx].Bitangent = B;
}