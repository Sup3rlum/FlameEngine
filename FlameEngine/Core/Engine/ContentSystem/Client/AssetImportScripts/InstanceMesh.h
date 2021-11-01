#pragma once


#include "../LocalContentService.h"

#include "Core/Engine/GameSystem/Environment/InstanceMesh.h" 
#include "Core/Engine/FlameRI/FRI.h"

template<>
EXPORT(struct, TContentSerializer<InstanceMesh>)
{
private:
	FRIVertexDeclaration * vertexDeclaration;
	FRIContext* renderContext;

public:
	TContentSerializer(FRIContext* friContext, const FArray<FRIVertexDeclarationComponent>& vertexDataLayout, const FArray<FRIVertexDeclarationComponent>& instanceDataLayout,const ShaderLibraryModulePart& signatureShader);
	InstanceMesh Serialize(IOFileStream& fileStream);
};


struct FInstanceMeshFileDescriptor : public FLocalContentFileDescriptionBase
{
	FInstanceMeshFileDescriptor() : FLocalContentFileDescriptionBase("FL3D", ELocalContentFileStorage::BINARY) {}
};


