#pragma once


#include "../AssetManager.h"


#include "Core/Engine/GameSystem/Environment/EnvironmentMap.h" 
#include "Core/Engine/FlameRI/FRI.h"

template<>
EXPORT(struct, TContentSerializer<EnvironmentMap>)
{

	TContentSerializer(IAssetManager* manager) : renderContext(manager->RenderContext) {}
	EnvironmentMap Serialize(Stream& fileStream);

private:

	FRICreationDescriptor ReadMap(Stream& fileStream);
	FRIContext* renderContext;
};

