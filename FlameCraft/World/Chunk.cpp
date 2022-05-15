#include "Chunk.h"
#include "World.h"

Chunk::Chunk()
{

}

Block Chunk::GetBlock(IVector3 pos)
{
	if (pos.x < 0 || pos.x > CHUNK_WIDTH - 1 ||
		pos.y < 0 || pos.y > CHUNK_HEIGHT - 1 ||
		pos.z < 0 || pos.z > CHUNK_DEPTH - 1)
		return Block(0);


	return data[CHUNK_AREA * pos.y + CHUNK_WIDTH * pos.z + pos.x];
}

void Chunk::SetBlock(Block block, IVector3 pos)
{
	if (pos.x < 0 || pos.x > CHUNK_WIDTH - 1 ||
		pos.y < 0 || pos.y > CHUNK_HEIGHT - 1 ||
		pos.z < 0 || pos.z > CHUNK_DEPTH - 1)
		return;

	data[CHUNK_AREA * pos.y + CHUNK_WIDTH * pos.z + pos.x] = block;
}

