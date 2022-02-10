
struct GSInput
{
    float4 Position : SV_Position;
    float4 Albedo : COLOR0;
};



struct PSInput
{
    float4 Position : SV_Position;
    float4 Albedo : COLOR0;
};


static float4 cubeVertices[8] =
{

    float4(0.5f, 0.5f, 0.5f, 0.0f),
	float4(0.5f, 0.5f, -0.5f, 0.0f),
	float4(0.5f, -0.5f, 0.5f, 0.0f),
	float4(0.5f, -0.5f, -0.5f, 0.0f),
	float4(-0.5f, 0.5f, 0.5f, 0.0f),
	float4(-0.5f, 0.5f, -0.5f, 0.0f),
	float4(-0.5f, -0.5f, 0.5f, 0.0f),
	float4(-0.5f, -0.5f, -0.5f, 0.0f)
};

static int cubeIndices[36] =
{
        0, 1, 2, 2, 1, 3, // right
		6, 7, 4, 4, 7, 5, // left
		5, 1, 4, 4, 1, 0, // up
		6, 7, 2, 2, 7, 3, // down
		4, 6, 0, 0, 6, 2, // front
		1, 3, 5, 5, 3, 7 // back
};



cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
    matrix InverseView;
    float4 CameraPosition;
};


cbuffer VXGIBuffer : register(b8)
{
    float maxTracingDistanceGlobal;
    float bounceStrength;
    float aoFalloff;
    float aoAlpha;
    float samplingFactor;
    float coneShadowTolerance;
    float coneShadowAperture;

    float voxelVolumeDimension;
    float sceneDimension;
    float voxelScale;
    float voxelSize;
}


[maxvertexcount(36)]
void main(
	point  GSInput input[1], 
	inout TriangleStream< PSInput > OutputStream
)
{

	
    float4 projectedVertices[8];
    
    for (int i = 0; i < 8; ++i)
    {
        float4 viewVertex = mul(View, input[0].Position + cubeVertices[i] * voxelSize);
       
        projectedVertices[i] = mul(Projection, viewVertex);
    }

	
    for (int face = 0; face < 12; ++face)
    {
		
        for (int vertex = 0; vertex < 3; ++vertex)
        {
            PSInput output;
						
            output.Position = projectedVertices[cubeIndices[face * 3 + vertex]];
            output.Albedo = input[0].Albedo;
            
            OutputStream.Append(output);
        }

        OutputStream.RestartStrip();
    }
}