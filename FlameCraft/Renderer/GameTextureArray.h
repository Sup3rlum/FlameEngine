#pragma once

#include "Core/Engine/FlameRI/FRI.h"
#include "../ImportScripts/BitTexture.h"
#include <FlameEngine/Core/Engine/ContentSystem/Client/LocalAssetManager.h>


struct GameTextureArray
{
	FRITexture2DArray* friTexture;
	GameTextureArray(FRICommandList& cmdList, FAssetManager& Content);
};