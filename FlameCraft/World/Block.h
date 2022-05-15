#pragma once


struct Block
{
	int ID;

	Block(int id) :
		ID(id)
	{}

	Block() : Block(0)
	{}
};

#define BLOCK_AIR 0	
#define BLOCK_STONE 1
#define BLOCK_GRASS_BLOCK 2
#define BLOCK_DIRT 3
#define BLOCK_COBBLE 4
#define BLOCK_PLANKS 5
#define BLOCK_SAND 6
#define BLOCK_OAK 7
#define BLOCK_LEAVES 8
#define BLOCK_WATER 9
#define BLOCK_GRASS 10