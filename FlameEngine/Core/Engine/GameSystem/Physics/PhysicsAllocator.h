#pragma once

#include "Core/Common/CoreCommon.h" 


#include "PhysicsShape.h"
#include "PhysicsMaterial.h"
#include "PhysicsComponent.h"
#include "PhysicsMesh.h"


class PhysicsAllocator
{
public:
	virtual TriangleMeshGeometry CookTriangleMeshGeometry(const PhysicsTriangleMeshDesc& desc) = 0;

	virtual RigidBody CreateDynamic(FTransform transform) = 0;
	virtual StaticRigidBody CreateStatic(FTransform transform) = 0;
	virtual CharacterBody CreateCharacter(FTransform transform) = 0;
	virtual VehicleBody CreateVehicle(FTransform transform) = 0;


	virtual bool RayCast(FRay ray, float distance, FVector3& outPoisition, FVector3& outNormal) = 0;

};