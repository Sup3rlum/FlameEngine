#include "WorldGenerator.h"

size_t RandomSeed()
{
	size_t high = rand();
	size_t low = rand();

	return (high << 32) + low;
}


WorldGenerator::WorldGenerator(size_t seed) : 
	Seed(seed),
	perlin(seed)
{

}

WorldGenerator::WorldGenerator() : 
	WorldGenerator(RandomSeed())
{}


int WorldGenerator::GenerateTerrain(FVector2 position)
{
	float fheight = perlin.octave2D_01(position.x / 128.0f, position.y / 128.0f, 4.0f) * 40.0f;

	return fheight;
}