#pragma once



#include "../PhysicsAllocator.h"

class FPXAllocator : public PhysicsAllocator
{
public:
	class FPXService* fpxService;
	class FPXScene* fpxScene;

	FPXAllocator(class FPXService* fpxser, class FPXScene* fpxsc) :
		fpxService(fpxser),
		fpxScene(fpxsc)
	{}

	TriangleMeshGeometry CookTriangleMeshGeometry(const PhysicsTriangleMeshDesc& desc);

	RigidBody CreateDynamic(FTransform transform);
	StaticRigidBody CreateStatic(FTransform transform);
	CharacterBody CreateCharacter(FTransform transform);

};