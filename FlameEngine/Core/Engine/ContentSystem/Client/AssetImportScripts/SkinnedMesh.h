#pragma once


#include "../LocalContentService.h"

#include "Core/Engine/GameSystem/SkinnedMesh.h" 
#include "Core/Engine/FlameRI/FRI.h"

template<>
EXPORT(struct, TContentSerializer<SkinnedMesh>)
{
private:

	FRIVertexDeclaration* vertexDeclaration;
	FRIContext* renderContext;

public:
	TContentSerializer(FRIContext* friContext);

	SkinnedMesh Serialize(IOFileStream& fileStream);
};


struct FSkinnedMeshFileDescriptor : public FLocalContentFileDescriptionBase
{
	FSkinnedMeshFileDescriptor() : FLocalContentFileDescriptionBase("FL3D", ELocalContentFileStorage::BINARY) {}
};

