#include "GameTextureArray.h"


#define TEXTURES_NUM 13

GameTextureArray::GameTextureArray(FRICommandList& cmdList, FAssetManager& Content)
{
	FString blockTextures[TEXTURES_NUM] =
	{
		"Materials/mc/dirt.flmt",
		"Materials/mc/stone.flmt",
		"Materials/mc/grass.flmt",
		"Materials/mc/grass_top.flmt",
		"Materials/mc/dirt.flmt",
		"Materials/mc/cobble.flmt",
		"Materials/mc/planks.flmt",
		"Materials/mc/sand.flmt",
		"Materials/mc/oak.flmt",
		"Materials/mc/oak_top.flmt",
		"Materials/mc/leaves.flmt",
		"Materials/mc/water.flmt",
		"Materials/mc/grass_bill.flmt"
	};

	FRICreationDescriptor textureData[TEXTURES_NUM];

	int width = 32;
	int height = 32;

	for (auto i : FRange(TEXTURES_NUM))
	{
		BitTexture tex = Content.Load<BitTexture>(blockTextures[i]);
		textureData[i] = tex.DataDescriptor;
	}

	this->friTexture = cmdList.GetDynamic()->CreateTexture2DArray(width, height, TEXTURES_NUM, EFRITextureFormat::RGBA8UNORM, FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::Byte), textureData);
}