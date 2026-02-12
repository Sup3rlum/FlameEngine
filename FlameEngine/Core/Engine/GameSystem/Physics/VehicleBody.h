#pragma once

#include "../EntityComponent/EntityComponent.h"
#include "Core/Math/Module.h"
#include "Core/Engine/Common/FTime.h"

#include "PhysicsShape.h"


EXPORT(struct, VehicleBody)
{
private:
	class FPXAllocator* Allocator;
	class PhysXVehicleProxy;
	PhysXVehicleProxy* pPxActor;
	friend class FPXAllocator;

	VehicleBody(PhysXVehicleProxy * pActor, class FPXAllocator* allocator) : pPxActor(pActor), Allocator(allocator) {}

public:


	void SetShape(const PhysShape & physShape);

	void SetLinearVelocity(FVector3 velocity);
	void SetAngularVelocity(FVector3 avelocity);

	void SetPosition(FVector3 position);
	void SetOrientation(FQuaternion orientation);

	void AddForce(FVector3 force);
	void AddTorque(FVector3 force);

	void ClearForce();
	void ClearTorque();


	void SetAngularDamping(float damping);
	void SetMass(float mass);


	void SetGlobalTransform(FTransform transform);
	FTransform GetGlobalTransform() const;


	bool IsAwake() const;
	void Release();

};
