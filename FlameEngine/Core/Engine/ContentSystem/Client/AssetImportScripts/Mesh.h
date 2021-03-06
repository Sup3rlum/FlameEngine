#pragma once


#include "../LocalContentService.h"

#include "Core/Engine/GameSystem/MeshComponent.h" 
#include "Core/Engine/FlameRI/FRI.h"

EXPORT(struct, FMeshSerializer) : public TSerializerInterface<MeshComponent>
{
private:
	FRIVertexDeclaration* vertexDeclaration;

public:
	FMeshSerializer(FRIContext* friContext);

	MeshComponent Serialize(IOFileStream& fileStream);

	FRIContext* renderContext;
};


struct FMeshFileDescriptor : public FLocalContentFileDescriptionBase
{
	FMeshFileDescriptor() : FLocalContentFileDescriptionBase("FL3D", ELocalContentFileStorage::BINARY) {}
};


typedef FLocalContentLoader<MeshComponent, FMeshFileDescriptor, FMeshSerializer> FMeshLoader;

