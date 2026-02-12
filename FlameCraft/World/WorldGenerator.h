#pragma once

#include <FlameEngine/Core/Math/Module.h>
#include "Perlin.h"
#include "Chunk.h"

struct WorldGenerator
{
	size_t Seed;
	WorldGenerator(size_t seed);
	WorldGenerator();

	Chunk* GenerateChunk(IVector2 vec);
	void GenerateStructure(Chunk* chunk, IVector3 position);

	siv::PerlinNoise perlin;

	int GenerateTerrain(FVector2 pos);
};
