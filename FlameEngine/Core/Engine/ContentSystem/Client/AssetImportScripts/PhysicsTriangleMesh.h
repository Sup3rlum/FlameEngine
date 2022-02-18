#pragma once


#include "../AssetManager.h"

#include "Core/Engine/GameSystem/Physics/PhysicsMesh.h" 

template<>
EXPORT(struct, TContentSerializer<PhysicsTriangleMeshDesc>)
{
private:

public:
	TContentSerializer() {}

	PhysicsTriangleMeshDesc Serialize(IOFileStream& fileStream);
};


struct FPhysTriangleMeshFileDescriptor : public FLocalContentFileDescriptionBase
{
	FPhysTriangleMeshFileDescriptor() : FLocalContentFileDescriptionBase("FL3D", ELocalContentFileStorage::BINARY) {}
};
