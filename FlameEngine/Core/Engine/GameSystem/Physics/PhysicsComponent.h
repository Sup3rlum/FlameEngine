#pragma once


#include "../EntityComponent/EntityComponent.h"
#include "Core/Math/Module.h"
#include "Core/Engine/Common/FTime.h"

#include "PhysicsShape.h"


EXPORT(struct, RigidBody)
{
private:
	class FPXAllocator* Allocator;
	class PhysXActorProxy;
	PhysXActorProxy* pPxActor;
	friend class FPXAllocator;

	RigidBody(PhysXActorProxy* pActor, class FPXAllocator* allocator) : pPxActor(pActor), Allocator(allocator) {}

public:


	void SetShape(const PhysShape& physShape);

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

};

EXPORT(struct, StaticRigidBody)
{
private:
	class FPXAllocator* Allocator;

	class PhysXActorProxy;
	PhysXActorProxy* pPxActor;
	friend class FPXAllocator;
	StaticRigidBody(PhysXActorProxy* pActor, class FPXAllocator* allocator) : pPxActor(pActor), Allocator(allocator) {}
public:

	void SetShape(const PhysShape& physShape);
	void RemoveShapes();
	void SetGlobalTransform(FTransform transform);
	FTransform GetGlobalTransform() const;

	bool IsAwake() const;
	bool HasPhysics() const;

};


EXPORT(struct, CharacterBody)
{
private:
	class FPXAllocator* Allocator;
	class PhysXControllerProxy;
	PhysXControllerProxy* pPxController;
	friend class FPXAllocator;

	FVector3 Velocity;
	FVector3 Gravity;
	FTimeSpan LastMoveTimestamp = 0;
	bool isGrounded = false;

	CharacterBody(PhysXControllerProxy * pController, class FPXAllocator* allocator) :
		pPxController(pController),
		Allocator(allocator),
		HasGravity(true)
	{
	}
public:

	bool HasGravity;

	void Move(FVector3 Velocity);
	FTransform GetGlobalTransform() const;

	FVector3 GetViewpoint(float factor) const;

	bool IsGrounded() const;
	float GetHeight() const;
};