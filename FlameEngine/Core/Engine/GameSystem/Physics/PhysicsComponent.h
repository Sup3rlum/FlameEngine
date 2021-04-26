#pragma once


#include "../EntityComponent/EntityComponent.h"
#include "Core/Math/Module.h"

#include "PhysicsShape.h"


EXPORT(struct, DynamicPhysicsComponent)
{
private:

	class PhysXActorProxy;
	PhysXActorProxy* pPxActor;
	friend class FPXAllocator;

	DynamicPhysicsComponent(PhysXActorProxy* pActor) : pPxActor(pActor) {}

public:


	void AttachShape(PhysicsShape* physShape);

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

	FTransform GetGlobalTransform() const;


	void SetGlobalTransform(FTransform transform);
	//void SetRelativeTransform(FActor& relative, FTransform transform);


	bool IsAwake() const;
	bool HasPhysics() const;

};

EXPORT(struct, StaticPhysicsComponent)
{
private:

	class PhysXActorProxy;
	PhysXActorProxy* pPxActor;
	friend class FPXAllocator;

	StaticPhysicsComponent(PhysXActorProxy* pActor) : pPxActor(pActor) {}
public:

	void AttachShape(PhysicsShape* physShape);
	FTransform GetGlobalTransform() const;


	void SetGlobalTransform(FTransform transform);

	bool IsAwake() const;
	bool HasPhysics() const;

};