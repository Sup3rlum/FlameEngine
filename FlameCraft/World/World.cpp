#include "World.h"
#include <iostream>
#include <mutex>

#define OBSERVERS_NOTIFY for (auto& observer : Observers) observer

std::mutex m;
std::mutex m2;

Block World::GetBlock(IVector3 pos)
{
	IVector2 chunkCoord = GlobalToChunkCoord(pos);

	if (!ChunkData.Contains(chunkCoord))
		return Block(0);

	auto chunkPtr = ChunkData[chunkCoord];

	IVector3 blockPosChunk = GlobalToChunk(pos);
	return chunkPtr->GetBlock(blockPosChunk);
}

void World::SetBlock(Block block, IVector3 pos, bool lazyUpdate)
{
	IVector2 chunkCoord = GlobalToChunkCoord(pos);

	if (!ChunkData.Contains(chunkCoord))
		return;

	auto chunkPtr = ChunkData[chunkCoord];

	IVector3 blockPosChunk = GlobalToChunk(pos);
	auto oldBlock = chunkPtr->GetBlock(blockPosChunk);
	chunkPtr->SetBlock(block, blockPosChunk);

	if (!lazyUpdate)
	{
		OBSERVERS_NOTIFY->BlockChanged(oldBlock, block, pos);
	}
}

void World::GenerateStructure(Chunk* chunk, IVector3 position)
{
	int treeHeight = 5;

	for (int dx = -2; dx <= 2; dx++)
	{
		for (int dz = -2; dz <= 2; dz++)
		{
			chunk->SetBlock(Block(BLOCK_LEAVES), position + IVector3(dx, 2, dz));
			chunk->SetBlock(Block(BLOCK_LEAVES), position + IVector3(dx, 3, dz));
		}
	}
	chunk->SetBlock(Block(BLOCK_LEAVES), position + IVector3(1, 4, 0));
	chunk->SetBlock(Block(BLOCK_LEAVES), position + IVector3(-1, 4, 0));
	chunk->SetBlock(Block(BLOCK_LEAVES), position + IVector3(0, 4, 1));
	chunk->SetBlock(Block(BLOCK_LEAVES), position + IVector3(0, 4, -1));
	
	chunk->SetBlock(Block(BLOCK_LEAVES), position + IVector3(1, 5, 0));
	chunk->SetBlock(Block(BLOCK_LEAVES), position + IVector3(-1, 5, 0));
	chunk->SetBlock(Block(BLOCK_LEAVES), position + IVector3(0, 5, 1));
	chunk->SetBlock(Block(BLOCK_LEAVES), position + IVector3(0, 5, -1));
	chunk->SetBlock(Block(BLOCK_LEAVES), position + IVector3(0, 5, 0));


	for (int i = 0; i < treeHeight; i++)
	{
		chunk->SetBlock(Block(BLOCK_OAK), position + IVector3(0, i, 0));
	}
}

void World::GenerateChunk(IVector2 chunkPos)
{
	if (ChunkData.Contains(chunkPos))
		return;

	auto newChunk = new Chunk();
	newChunk->Position = chunkPos;


	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			float blockX = x + chunkPos.x * 16.0f;
			float blockZ = z + chunkPos.y * 16.0f;

			int height = worldGenerator->GenerateTerrain(FVector2(blockX, blockZ));

			for (int y = 0; y < height; y++)
			{
				Block block(BLOCK_AIR);

				if (height > 20)
				{
					if (y == height - 1)
						block.ID = BLOCK_GRASS_BLOCK;
					else if (y > height - 3)
						block.ID = BLOCK_DIRT;
					else
						block.ID = BLOCK_STONE;

				}
				else
				{
					if (y == height - 1)
						block.ID = BLOCK_SAND;
					else
						block.ID = BLOCK_STONE;
				}

				newChunk->SetBlock(block, IVector3(x, y, z));
			}

			for (int y = height; y < 20; y++)
			{
				newChunk->SetBlock(BLOCK_WATER, IVector3(x, y, z));
			}
			
			if (rand() % 500 == 1 && height > 20)
			{
				GenerateStructure(newChunk, IVector3(x, height, z));
			}
			if (rand() % 30 == 1 && height > 20)
			{
				newChunk->SetBlock(BLOCK_GRASS, IVector3(x, height, z));
			}
		}
	}

	ChunkData[chunkPos] = newChunk;

	ScheduleUpdate(chunkPos, true);
}

void World::ScheduleUpdate(IVector2 chunkPos, bool threadPool)
{
	OBSERVERS_NOTIFY->ChunkScheduledUpdate(chunkPos, threadPool);
}



void World::Tick()
{
	OBSERVERS_NOTIFY->WorldTick();
}


World::World() :
	worldGenerator(new WorldGenerator(12345))
{
}

World::~World()
{
	for (auto [chunkPos, chunk] : ChunkData)
		delete chunk;
}