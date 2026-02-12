
struct PSInput
{
    float4 Position : SV_Position;
    float3 vDirection : VDIRECTION0;
};


TextureCube EnvMap : register(t10);
SamplerState EnvSampler : register(s10);


float4 main(PSInput input) : SV_TARGET
{
    //float3 upRay = float3(0,1,0);
    //float3 skyRay = float3(input.vDirection.x, input.vDirection.y * 15, input.vDirection.z);
    
    //float fCos = dot(normalize(upRay), normalize(skyRay));
    
    //float3 finalColor = float3(0, 0, 0);   
    //float comp = (float) (skyRay.y > 0);
    
    float4 finalColor = EnvMap.Sample(EnvSampler, input.vDirection); //lerp(SecondaryColor.rgb, PrimaryColor.rgb, fCos * comp);
    
   
    return finalColor;
}