#pragma once



#include "../PhysicsAllocator.h"

EXPORT(class, FPXAllocator) : public PhysicsAllocator
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
	VehicleBody CreateVehicle(FTransform transform);

	bool RayCast(FRay ray, float distance, FVector3& outPosition, FVector3& outNormal) override;

};