#pragma once


#include "../LocalContentService.h"

#include "Core/Engine/GameSystem/SkinnedMeshComponent.h" 
#include "Core/Engine/FlameRI/FRI.h"

EXPORT(struct, FSkinnedMeshSerializer) : public TSerializerInterface<SkinnedMeshComponent>
{

	FSkinnedMeshSerializer(FRIContext * friContext) : renderContext(friContext) {}

	SkinnedMeshComponent Serialize(IOFileStream & fileStream);

	FRIContext* renderContext;
};


struct FSkinnedMeshFileDescriptor : public FLocalContentFileDescriptionBase
{
	FSkinnedMeshFileDescriptor() : FLocalContentFileDescriptionBase("FL3D", ELocalContentFileStorage::BINARY) {}
};


typedef FLocalContentLoader <SkinnedMeshComponent, FSkinnedMeshFileDescriptor, FSkinnedMeshSerializer> FSkinnedMeshLoader;