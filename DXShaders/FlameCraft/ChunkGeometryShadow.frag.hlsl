struct PSInput
{
    float4 Position : SV_Position;
};


float4 main(PSInput input) : SV_Target0
{
    float clipDepth = input.Position.z;
    float moments2 = clipDepth * clipDepth;
    
    return float4(clipDepth, moments2, 0.0f, 1.0f);

}