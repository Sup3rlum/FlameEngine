


struct GSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
    float4 WorldPosition : WORLDPOS0;
	
};



[maxvertexcount(3)]
void main(
	triangle GSInput input[3],
	inout TriangleStream<GSInput> output
)
{
	
	
    float3 p1 = input[1].WorldPosition.xyz - input[0].WorldPosition.xyz;
    float3 p2 = input[2].WorldPosition.xyz - input[0].WorldPosition.xyz;
	
    float3 n = cross(p1, p2);
    float3 p = abs(n);
   
    
    
    // Get Dominant axis and project triangle
    float4 projectedPos[3] =
    {
        float4(0, 0, 0, 0),
        float4(0, 0, 0, 0),
        float4(0, 0, 0, 0)
    };
    
 
    
    if (p.z > p.x && p.z > p.y)
    {
        projectedPos[0] = float4(input[0].Position.x, input[0].Position.y, 0, 1);
        projectedPos[1] = float4(input[1].Position.x, input[1].Position.y, 0, 1);
        projectedPos[2] = float4(input[2].Position.x, input[2].Position.y, 0, 1);
    }
    else if (p.x > p.y && p.x > p.z)
    {
        projectedPos[0] = float4(input[0].Position.y, input[0].Position.z, 0, 1);
        projectedPos[1] = float4(input[1].Position.y, input[1].Position.z, 0, 1);
        projectedPos[2] = float4(input[2].Position.y, input[2].Position.z, 0, 1);
    }
    else
    {
        projectedPos[0] = float4(input[0].Position.x, input[0].Position.z, 0, 1);
        projectedPos[1] = float4(input[1].Position.x, input[1].Position.z, 0, 1);
        projectedPos[2] = float4(input[2].Position.x, input[2].Position.z, 0, 1);
    }
    
    
    
    // If
    
    int3 indices = int3(0, 1, 2); 
    if (dot(n, float3(0.0, 0.0, 1.0)) > 0.0)
    {
        indices = int3(0, 2, 1);
    }
    
	
	[unroll]
	for (uint i = 0; i < 3; i++)
	{
        GSInput element;
		element.TexCoord = input[indices[i]].TexCoord;
        element.Normal = input[indices[i]].Normal;
        element.Tangent = input[indices[i]].Tangent;
        element.Bitangent = input[indices[i]].Bitangent;
        element.WorldPosition = input[indices[i]].WorldPosition;
        element.Position = projectedPos[indices[i]];
        
		output.Append(element);
	}

    
}