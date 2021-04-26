#include "PhysicsComponent.h"



#include "PX/FPXActorProxy.h"



void DynamicPhysicsComponent::AddForce(FVector3 force)
{
	pPxActor->addForce(physx_cast(force));
}

void DynamicPhysicsComponent::AddTorque(FVector3 torque)
{
	pPxActor->addForce(physx_cast(torque));
}

void DynamicPhysicsComponent::SetLinearVelocity(FVector3 velocity)
{
	pPxActor->setLinearVelocity(physx_cast(velocity));
}
void DynamicPhysicsComponent::SetAngularVelocity(FVector3 velocity)
{
	pPxActor->setAngularVelocity(physx_cast(velocity));

}


void DynamicPhysicsComponent::ClearForce()
{
	pPxActor->clearForce();
}

void DynamicPhysicsComponent::ClearTorque()
{
	pPxActor->clearTorque();
}


void DynamicPhysicsComponent::SetPosition(FVector3 position)
{
	pPxActor->setGlobalPose(
		PxTransform(
			physx_cast(position),
			pPxActor->getGlobalPose().q
		)
	);
}

void DynamicPhysicsComponent::SetOrientation(FQuaternion orientation)
{
	pPxActor->setGlobalPose(
		PxTransform(
			pPxActor->getGlobalPose().p,
			physx_cast(orientation)
		)
	);
}


void DynamicPhysicsComponent::SetGlobalTransform(FTransform transform)
{
	pPxActor->setGlobalPose(
		physx_cast(transform)
	);
}


FTransform DynamicPhysicsComponent::GetGlobalTransform() const
{
	FVector3 pos = physx_cast(pPxActor->getGlobalPose().p);
	FQuaternion quat = physx_cast(pPxActor->getGlobalPose().q);

	return FTransform(pos, quat);
}

void DynamicPhysicsComponent::AttachShape(PhysicsShape* physShape)
{
	pPxActor->attachShape(*(((FPXShape*)physShape)->pxShape));
}



void DynamicPhysicsComponent::SetAngularDamping(float f)
{
	pPxActor->setAngularDamping(f);
}

void DynamicPhysicsComponent::SetMass(float f)
{
	pPxActor->setMass(f);
}


/*
* 
*	Static
* 
*/




void StaticPhysicsComponent::SetGlobalTransform(FTransform transform)
{
	pPxActor->setGlobalPose(
		physx_cast(transform)
	);
}


FTransform StaticPhysicsComponent::GetGlobalTransform() const
{
	return physx_cast(pPxActor->getGlobalPose());
}

void StaticPhysicsComponent::AttachShape(PhysicsShape* physShape)
{
	pPxActor->attachShape(*(((FPXShape*)physShape)->pxShape));
}

