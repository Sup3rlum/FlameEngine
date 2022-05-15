#pragma once

#include <FlameEngine/Core/Math/Module.h>
#include "Perlin.h"


struct WorldGenerator
{
	size_t Seed;
	WorldGenerator(size_t seed);
	WorldGenerator();

	siv::PerlinNoise perlin;

	int GenerateTerrain(FVector2 pos);
};
