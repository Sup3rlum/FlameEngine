

Texture3D<unorm float4> RadianceVolume : register(t0);
RWTexture3D<unorm float4> VoxelMips[6] : register(u0);

static int3 anisoOffsets[] =
{
    int3(1, 1, 1),
	int3(1, 1, 0),
	int3(1, 0, 1),
	int3(1, 0, 0),
	int3(0, 1, 1),
	int3(0, 1, 0),
	int3(0, 0, 1),
	int3(0, 0, 0)
};

void FetchTexels(int3 pos, inout float4 val[8])
{
    for (int i = 0; i < 8; i++)
    {
        val[i] = RadianceVolume.Load(int4(pos + anisoOffsets[i], 0));
    }
}


void StoreFiltering(RWTexture3D<unorm float4> Grid, int3 gridPos, float4 value)
{
    Grid[gridPos] = value;
}

[numthreads(8, 8, 8)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    int3 writePos = DTid;
    int3 sourcePos = writePos * 2;
    float4 values[8];

    FetchTexels(sourcePos, values);
	// x -
    StoreFiltering(VoxelMips[0], writePos,
	(
		values[0] + values[4] * (1 - values[0].a) +
		values[1] + values[5] * (1 - values[1].a) +
		values[2] + values[6] * (1 - values[2].a) +
		values[3] + values[7] * (1 - values[3].a)) * 0.25f
	);
	// x +
    StoreFiltering(VoxelMips[1], writePos,
	(
		values[4] + values[0] * (1 - values[4].a) +
    	values[5] + values[1] * (1 - values[5].a) +
    	values[6] + values[2] * (1 - values[6].a) +
    	values[7] + values[3] * (1 - values[7].a)) * 0.25f
    );
	// y -
    StoreFiltering(VoxelMips[2], writePos,
	(
		values[0] + values[2] * (1 - values[0].a) +
    	values[1] + values[3] * (1 - values[1].a) +
    	values[5] + values[7] * (1 - values[5].a) +
    	values[4] + values[6] * (1 - values[4].a)) * 0.25f
    );
	// y +
    StoreFiltering(VoxelMips[3], writePos,
	(
		values[2] + values[0] * (1 - values[2].a) +
    	values[3] + values[1] * (1 - values[3].a) +
    	values[7] + values[5] * (1 - values[7].a) +
    	values[6] + values[4] * (1 - values[6].a)) * 0.25f
    );
	// z -
    StoreFiltering(VoxelMips[4], writePos,
	(
		values[0] + values[1] * (1 - values[0].a) +
    	values[2] + values[3] * (1 - values[2].a) +
    	values[4] + values[5] * (1 - values[4].a) +
    	values[6] + values[7] * (1 - values[6].a)) * 0.25f
    );
	// z +
    StoreFiltering(VoxelMips[5], writePos,
	(
		values[1] + values[0] * (1 - values[1].a) +
    	values[3] + values[2] * (1 - values[3].a) +
    	values[5] + values[4] * (1 - values[5].a) +
    	values[7] + values[6] * (1 - values[7].a)) * 0.25f
    );
}
