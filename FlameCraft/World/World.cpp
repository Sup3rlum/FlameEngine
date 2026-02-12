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


void World::GenerateChunk(IVector2 chunkPos)
{
	if (ChunkData.Contains(chunkPos))
		return;

	auto newChunk = worldGenerator->GenerateChunk(chunkPos);
	ChunkData[chunkPos] = newChunk;
	ScheduleUpdate(chunkPos, true);
}


void World::GenerateArea(IVector2 start, IVector2 end, bool immediate)
{
	int minX = min(start.x, end.x);
	int minY = min(start.y, end.y);

	int maxX = max(start.x, end.x);
	int maxY = max(start.y, end.y);

	for (int x = minX; x < maxX; x++)
	{
		for (int y = minY; y < maxY; y++)
		{
			IVector2 chunkPos(x, y);

			if (ChunkData.Contains(chunkPos))
				return;

			auto newChunk = worldGenerator->GenerateChunk(chunkPos);
			ChunkData[chunkPos] = newChunk;
			ScheduleUpdate(chunkPos, true);
		}
	}

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
	worldGenerator(new WorldGenerator(12345)),
	scene(0)
{
}

World::~World()
{
	for (auto [chunkPos, chunk] : ChunkData)
		delete chunk;
}

bool World::IsChunkLoaded(IVector2 vector)
{
	return ChunkData.Contains(vector);
}
