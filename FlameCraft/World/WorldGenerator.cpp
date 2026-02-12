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

Chunk* WorldGenerator::GenerateChunk(IVector2 chunkPos)
{
	auto newChunk = new Chunk();
	newChunk->Position = chunkPos;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			float blockX = x + chunkPos.x * 16.0f;
			float blockZ = z + chunkPos.y * 16.0f;

			int height = GenerateTerrain(FVector2(blockX, blockZ));

			for (int y = 0; y < height; y++)
			{
				Block block(Block::AIR);

				if (height > 20)
				{
					if (y == height - 1)
						block = Block::GRASS_BLOCK;
					else if (y > height - 3)
						block = Block::DIRT;
					else
						block = Block::STONE;

				}
				else
				{
					if (y == height - 1)
						block = Block::SAND;
					else
						block = Block::STONE;
				}

				newChunk->SetBlock(block, IVector3(x, y, z));
			}

			for (int y = height; y < 20; y++)
			{
				newChunk->SetBlock(Block::WATER, IVector3(x, y, z));
			}

			if (rand() % 500 == 1 && height > 20)
			{
				GenerateStructure(newChunk, IVector3(x, height, z));
			}
			if (rand() % 30 == 1 && height > 20)
			{
				newChunk->SetBlock(Block::GRASS, IVector3(x, height, z));
			}
		}
	}

	return newChunk;
}

void WorldGenerator::GenerateStructure(Chunk* chunk, IVector3 position)
{
	int treeHeight = 5;

	for (int dx = -2; dx <= 2; dx++)
	{
		for (int dz = -2; dz <= 2; dz++)
		{
			chunk->SetBlock(Block(Block::LEAVES), position + IVector3(dx, 2, dz));
			chunk->SetBlock(Block(Block::LEAVES), position + IVector3(dx, 3, dz));
		}
	}
	chunk->SetBlock(Block(Block::LEAVES), position + IVector3(1, 4, 0));
	chunk->SetBlock(Block(Block::LEAVES), position + IVector3(-1, 4, 0));
	chunk->SetBlock(Block(Block::LEAVES), position + IVector3(0, 4, 1));
	chunk->SetBlock(Block(Block::LEAVES), position + IVector3(0, 4, -1));

	chunk->SetBlock(Block(Block::LEAVES), position + IVector3(1, 5, 0));
	chunk->SetBlock(Block(Block::LEAVES), position + IVector3(-1, 5, 0));
	chunk->SetBlock(Block(Block::LEAVES), position + IVector3(0, 5, 1));
	chunk->SetBlock(Block(Block::LEAVES), position + IVector3(0, 5, -1));
	chunk->SetBlock(Block(Block::LEAVES), position + IVector3(0, 5, 0));


	for (int i = 0; i < treeHeight; i++)
	{
		chunk->SetBlock(Block(Block::OAK), position + IVector3(0, i, 0));
	}
}