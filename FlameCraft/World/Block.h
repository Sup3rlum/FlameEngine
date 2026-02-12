#pragma once


struct Block
{
	int ID;

	Block(int id) :
		ID(id)
	{}

	Block() : Block(0)
	{}

	static Block AIR;
	static Block STONE;
	static Block GRASS_BLOCK;
	static Block DIRT;
	static Block COBBLE;
	static Block PLANKS;
	static Block SAND;
	static Block OAK;
	static Block LEAVES;
	static Block WATER;
	static Block GRASS;

	friend static bool operator==(const Block& b1, const Block& b2)
	{
		return b1.ID == b2.ID;
	}

	bool IsTransparent() const
	{
		return *this == AIR || *this == WATER || *this == LEAVES || *this == GRASS;
	}

};

