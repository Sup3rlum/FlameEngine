#pragma once

#include "Block.h"
#include <FlameEngine/Core/Math/Module.h>

class World;

class WorldObserver
{
public:
	WorldObserver(World* _World) : _World(_World) {}

	virtual void BlockChanged(Block oldBlock, Block newBlock, IVector3 globalPos) = 0;
	virtual void ChunkScheduledUpdate(IVector2 chunkCoord, bool threadPool) = 0;
	virtual void WorldTick() = 0;

	World* _World;
};