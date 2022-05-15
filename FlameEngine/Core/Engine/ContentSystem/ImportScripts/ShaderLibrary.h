#pragma once


#include "../AssetManager.h"

#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"

template<>
EXPORT(struct, TContentSerializer<ShaderLibrary>)
{
	TContentSerializer(IAssetManager* manager) : renderContext(manager->RenderContext) {}
	ShaderLibrary Serialize(Stream & fileStream);

	FRIContext* renderContext;
};
