#pragma once
#include <FlameEngine/Core/Engine/GameSystem/RenderObject.h>
#include <FlameEngine/Core/Framework/IO/Stream.h>

#include "Block.h"

constexpr const int CHUNK_WIDTH = 16;
constexpr const int CHUNK_DEPTH = 16;
constexpr const int CHUNK_HEIGHT = 64;
constexpr const int CHUNK_AREA = CHUNK_WIDTH * CHUNK_DEPTH;
constexpr const int CHUNK_BLOCKS = CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT;

class Chunk
{
public:
	Block data[CHUNK_BLOCKS] = { 0 };
	IVector2 Position;

	Chunk();
	Block GetBlock(IVector3 pos);
	void SetBlock(Block block, IVector3 pos);
};