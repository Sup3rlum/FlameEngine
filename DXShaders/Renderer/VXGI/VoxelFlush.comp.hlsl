


RWTexture3D<uint> g_VoxelAlbedo : register(u0);
RWTexture3D<uint> g_VoxelNormal : register(u1);
RWTexture3D<uint> g_VoxelDetail : register(u2);
RWTexture3D<unorm float4> g_VoxelLit : register(u3);
RWTexture3D<unorm float4> g_VoxelFirstBounce : register(u4);



[numthreads(8, 8, 8)]
void main(uint3 DTid : SV_DispatchThreadID)
{

    g_VoxelLit[DTid] = 0; 
    g_VoxelFirstBounce[DTid] = 0;
    
    g_VoxelAlbedo[DTid] = 0;
    g_VoxelNormal[DTid] = 0;
    g_VoxelDetail[DTid] = 0;
}