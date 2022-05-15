#pragma once


#include "../AssetManager.h"


#include "Core/Engine/GameSystem/Material/MaterialComponent.h" 
#include "Core/Engine/FlameRI/FRI.h"

template<>
EXPORT(struct, TContentSerializer<Material>)
{

	TContentSerializer(IAssetManager* manager) : renderContext(manager->RenderContext) {}
	Material Serialize(Stream& fileStream);

	FRIContext* renderContext;
};

