#pragma once


#include <FlameEngine/Core/Engine/ContentSystem/AssetManager.h>
#include "Core/Engine/GameSystem/Material/MaterialComponent.h" 
#include "../Renderer/BitTexture.h"


template<>
struct TContentSerializer<BitTexture>
{

	TContentSerializer(IAssetManager * manager) : renderContext(manager->RenderContext) {}
	BitTexture Serialize(Stream & fileStream);

	FRIContext* renderContext;
};
