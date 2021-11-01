struct PSInput
{
    float4 Position : SV_Position;
    float4 Color : COLOR0;
    float3 Normal : NORMAL0;
};

struct PSOutput
{
    float4 Depth : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Albedo : SV_Target2;
    float4 Specular : SV_Target3;
    
};

float3 PackNormal(float3 n)
{
    return (normalize(n) + 1.0f) / 2.0f;
}

PSOutput main(PSInput input)
{
    PSOutput output;
    
    float clipDepth = input.Position.z;
    float moment2 = clipDepth * clipDepth;
    

    output.Depth = float4(clipDepth, moment2, 0.0f, 1.0f);
    output.Normal = float4(PackNormal(input.Normal), 1.0f);
    output.Albedo = float4(input.Color.rgb, 1.0f);
    output.Specular = float4(1, 1, 1, 1);
    
    return output;
}