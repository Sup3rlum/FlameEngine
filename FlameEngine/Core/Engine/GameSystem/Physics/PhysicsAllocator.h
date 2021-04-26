#pragma once

#include "Core/Common/CoreCommon.h" 


#include "PhysicsShape.h"
#include "PhysicsMaterial.h"
#include "PhysicsComponent.h"



class PhysicsAllocator
{
public:
	virtual PhysicsShape* CreateShape(PhysicsMaterial material, BoxGeometry geometry) = 0;
	virtual PhysicsShape* CreateShape(PhysicsMaterial material, SphereGeometry geometry) = 0;
	virtual PhysicsShape* CreateShape(PhysicsMaterial material, PlaneGeometry geometry) = 0;
	virtual PhysicsShape* CreateShape(PhysicsMaterial material, CapsuleGeometry geometry) = 0;


	virtual DynamicPhysicsComponent CreateDynamic(FTransform transform) = 0;
	virtual StaticPhysicsComponent CreateStatic(FTransform transform) = 0;

};