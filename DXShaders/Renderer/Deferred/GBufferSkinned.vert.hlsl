

static int MAX_JOINTS = 32;
static int MAX_WEIGHTS = 4;

struct VSInput
{
    float3 Position : POSITION0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 Bitangent : BITANGENT0;
    float2 TexCoord : TEXCOORD0;  
    
    int4 JointIndices : JOINT_INDICES0;
    float4 JointWeights : JOINT_WEIGHTS0;
};


struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL0;
    float4 ViewPosition : VIEWPOS0;
};


cbuffer CameraConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};


cbuffer TransformationBuffer : register(b1)
{
    matrix World;
    matrix WorldInverseTranspose;
}

cbuffer JointsBuffer : register(b3)
{
    matrix JointTransforms[32];
}

PSInput main(VSInput input)
{
    PSInput output;
	
   
    float4 totalLocalPos = float4(0.0, 0.0, 0.0, 0.0);
    float4 totalNormal = float4(0.0, 0.0, 0.0, 0.0);
   /* float4 totalTangent = float4(0.0, 0.0, 0.0, 0.0);
    float4 totalBitangent = float4(0.0, 0.0, 0.0, 0.0);*/
	

    for (int i = 0; i < MAX_WEIGHTS; i++)
    {
        matrix jointTransform = input.JointWeights[i] * JointTransforms[input.JointIndices[i]];

        totalLocalPos += mul(jointTransform, float4(input.Position, 1.0));
        totalNormal += mul(jointTransform, float4(input.Normal, 0.0));
       /* totalTangent += mul(jointTransform, float4(input.Tangent, 0.0));
        totalBitangent += mul(jointTransform, float4(input.Bitangent, 0.0));*/

    }

    output.TexCoord = input.TexCoord;
	
	
    float3x3 normalMatrix = mul((float3x3) View, (float3x3) WorldInverseTranspose);
	
   /* float3 fNormal = mul(normalMatrix, totalNormal.xyz);
    float3 fTangent = mul(normalMatrix, totalTangent.xyz);
    float3 fBitangent = mul(normalMatrix, totalBitangent.xyz);
    output.TBN = float3x3(fTangent, fBitangent, fNormal);*/
    output.Normal = mul(normalMatrix, totalNormal.xyz);
	
    float4 worldPos = mul(World, totalLocalPos);
    float4 viewPos = mul(View, worldPos);
    output.Position = mul(Projection, viewPos);
	
    return output;
}