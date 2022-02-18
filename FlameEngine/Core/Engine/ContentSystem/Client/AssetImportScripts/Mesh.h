#pragma once


#include "../AssetManager.h"

#include "Core/Engine/GameSystem/Mesh.h" 
#include "Core/Engine/FlameRI/FRI.h"

template<>
EXPORT(struct, TContentSerializer<Mesh>)
{
private:
	FRIVertexDeclaration* vertexDeclaration;
	FRIContext* renderContext;

public:
	TContentSerializer(FRIContext* friContext);

	Mesh Serialize(IOFileStream& fileStream);


};


struct FMeshFileDescriptor : public FLocalContentFileDescriptionBase
{
	FMeshFileDescriptor() : FLocalContentFileDescriptionBase("FL3D", ELocalContentFileStorage::BINARY) {}
};