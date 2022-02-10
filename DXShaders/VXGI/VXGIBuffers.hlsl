cbuffer VXGIBuffer : register(b10)
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
    
    float4 worldMinPoint;
    float4 worldMaxPoint;
    
    float _padding[1];
}



float3 VoxelToWorld(int3 vox)
{
   
    return float3(vox) * voxelSize + worldMinPoint.xyz;

}

float3 WorldToVoxel(float3 worldPos)
{
    return (worldPos - worldMinPoint.xyz) * voxelScale;
}

