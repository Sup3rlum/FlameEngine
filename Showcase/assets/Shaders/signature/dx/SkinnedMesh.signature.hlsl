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



float4 main(VSInput input) : SV_POSITION
{
    return float4(input.Position, 1.0f);
}