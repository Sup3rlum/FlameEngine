


const int MAX_JOINTS = 32;
const int MAX_WEIGHTS = 4;


struct VSInput
{
    float3 Position : POSITION0;
    int4 JointIndices : POSITION1;
    float4 JointWeights : POSITION2;
};

struct PSInput
{
    float4 Position : SV_Position;
};


cbuffer CameraMatrices : register(b0)
{
    matrix View;
    matrix Projection;
}

cbuffer TransformBuffer : register(b1)
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

    for (int i = 0; i < MAX_WEIGHTS; i++)
    {
        matrix jointTransform = input.JointWeights[i] * JointTransforms[input.JointIndices[i]];

        totalLocalPos += mul(jointTransform, float4(input.Position, 1.0));
    }
    
    
    float4 WorldPos = mul(World, totalLocalPos);
    float4 ViewPos = mul(View, WorldPos);
    output.Position = mul(Projection, ViewPos);
   
    return output;
}