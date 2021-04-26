#pragma once


#include "../LocalContentService.h"


#include "Core/Engine/GameSystem/Material/MaterialComponent.h" 
#include "Core/Engine/FlameRI/FRI.h"

EXPORT(struct, FMaterialSerializer) : public TSerializerInterface<MaterialComponent>
{

	FMaterialSerializer(FRIContext * friContext) : renderContext(friContext) {}

	MaterialComponent Serialize(IOFileStream & fileStream);

	FRIContext* renderContext;
};


struct FMaterialFileDescriptor : public FLocalContentFileDescriptionBase
{
	FMaterialFileDescriptor() : FLocalContentFileDescriptionBase("FLMT", ELocalContentFileStorage::BINARY) {}
};


typedef FLocalContentLoader<MaterialComponent, FMaterialFileDescriptor, FMaterialSerializer> FMaterialLoader;