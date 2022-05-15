#pragma once

#include "Chunk.h"
#include "WorldObserver.h"
#include "WorldGenerator.h"

struct ChunkIDHasher
{
	size_t operator()(const IVector2& vec)
	{
		size_t a = vec.x;
		size_t b = vec.y;

		size_t A = a >= 0 ? 2 * a : -2 * a - 1;
		size_t B = b >= 0 ? 2 * b : -2 * b - 1;
		
		size_t hash = A >= B ? A * A + A + B : A + B * B;

		return hash;
	}
};

struct World
{
	FHashMap<IVector2, Chunk*, ChunkIDHasher> ChunkData;
	FArray<WorldObserver*> Observers;
	WorldGenerator* worldGenerator;
	class Scene* scene;

	FVector3 playerPos;
	FVector3 playerLookDir;

	World();

	void GenerateChunk(IVector2 chunkPos);
	Block GetBlock(IVector3 pos);
	void SetBlock(Block block, IVector3 pos, bool lazyUpdate = false);
	void UpdateChunk(IVector2 chunkCoord);


	void GenerateStructure(Chunk* chunk, IVector3 position);
	void Tick();


	void ScheduleUpdate(IVector2 position, bool threadPool);


	FORCEINLINE static IVector2 GlobalToChunkCoord(IVector3 globalPos)
	{
		auto _chunkPos = globalPos;
		if (_chunkPos.x < 0)
			_chunkPos.x -= 15;
		if (_chunkPos.z < 0)
			_chunkPos.z -= 15;

		return IVector2(_chunkPos.x / CHUNK_WIDTH, _chunkPos.z / CHUNK_DEPTH);
	}
	FORCEINLINE static IVector3 GlobalToChunk(IVector3 globalPos)
	{
		return IVector3(
			globalPos.x & (CHUNK_WIDTH - 1),
			globalPos.y,
			globalPos.z & (CHUNK_DEPTH - 1));
	}
	FORCEINLINE static IVector3 ChunkToGlobal(IVector2 chunkPos, IVector3 blockPosChunk)
	{
		return World::ChunkToGlobal(chunkPos) + blockPosChunk;
	}

	FORCEINLINE static IVector3 ChunkToGlobal(IVector2 chunkPos)
	{
		return IVector3(chunkPos.x * CHUNK_WIDTH, 0, chunkPos.y * CHUNK_DEPTH);
	}

	~World();
};

