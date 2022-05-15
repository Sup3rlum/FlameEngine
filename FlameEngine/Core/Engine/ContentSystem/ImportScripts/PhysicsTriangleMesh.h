#pragma once


#include "../AssetManager.h"

#include "Core/Engine/GameSystem/Physics/PhysicsMesh.h" 

template<>
EXPORT(struct, TContentSerializer<PhysicsTriangleMeshDesc>)
{
private:

public:
	TContentSerializer(IAssetManager* manager) {}
	PhysicsTriangleMeshDesc Serialize(Stream& fileStream);
};
