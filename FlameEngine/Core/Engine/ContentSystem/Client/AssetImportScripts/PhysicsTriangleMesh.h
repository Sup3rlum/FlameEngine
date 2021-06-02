#pragma once


#include "../LocalContentService.h"

#include "Core/Engine/GameSystem/Physics/PhysicsMesh.h" 

EXPORT(struct, FPhysTriangleMeshSerializer) : public TSerializerInterface<PhysicsTriangleMeshDesc>
{
private:

public:
	FPhysTriangleMeshSerializer() {}

	PhysicsTriangleMeshDesc Serialize(IOFileStream& fileStream);
};


struct FPhysTriangleMeshFileDescriptor : public FLocalContentFileDescriptionBase
{
	FPhysTriangleMeshFileDescriptor() : FLocalContentFileDescriptionBase("FL3D", ELocalContentFileStorage::BINARY) {}
};


typedef FLocalContentLoader<PhysicsTriangleMeshDesc, FPhysTriangleMeshFileDescriptor, FPhysTriangleMeshSerializer> FPhysTriangleMeshLoader;

