#pragma once


#include "../LocalContentService.h"


#include "Core/Engine/GameSystem/Material/MaterialComponent.h" 
#include "Core/Engine/FlameRI/FRI.h"

template<>
EXPORT(struct, TContentSerializer<Material>)
{

	TContentSerializer(FRIContext * friContext) : renderContext(friContext) {}
	Material Serialize(IOFileStream & fileStream);

	FRIContext* renderContext;
};


struct FMaterialFileDescriptor : public FLocalContentFileDescriptionBase
{
	FMaterialFileDescriptor() : FLocalContentFileDescriptionBase("FLMT", ELocalContentFileStorage::BINARY) {}
};
