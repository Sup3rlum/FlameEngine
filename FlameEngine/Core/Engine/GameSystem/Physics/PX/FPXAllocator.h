#pragma once



#include "../PhysicsAllocator.h"
#include "FPXShape.h"


class FPXAllocator : public PhysicsAllocator
{
public:
	class FPXService* fpxService;
	class FPXScene* fpxScene;

	FPXAllocator(class FPXService* fpxser, class FPXScene* fpxsc) :
		fpxService(fpxser),
		fpxScene(fpxsc)
	{}


	PhysicsShape* CreateShape(PhysicsMaterial material, BoxGeometry geometry);
	PhysicsShape* CreateShape(PhysicsMaterial material, SphereGeometry geometry);
	PhysicsShape* CreateShape(PhysicsMaterial material, PlaneGeometry geometry);
	PhysicsShape* CreateShape(PhysicsMaterial material, CapsuleGeometry geometry);

	DynamicPhysicsComponent CreateDynamic(FTransform transform);
	StaticPhysicsComponent CreateStatic(FTransform transform);
};