#include "PhysicsComponent.h"



#include "PX/FPXActorProxy.h"
#include "PX/FPXAllocator.h"
#include "PX/FPXService.h"
#include "PhysicsGeometryProxy.h"


void RigidBody::AddForce(FVector3 force)
{
	pPxActor->addForce(physx_cast(force));
}

void RigidBody::AddTorque(FVector3 torque)
{
	pPxActor->addForce(physx_cast(torque));
}

void RigidBody::SetLinearVelocity(FVector3 velocity)
{
	pPxActor->setLinearVelocity(physx_cast(velocity));
}
void RigidBody::SetAngularVelocity(FVector3 velocity)
{
	pPxActor->setAngularVelocity(physx_cast(velocity));

}


void RigidBody::ClearForce()
{
	pPxActor->clearForce();
}

void RigidBody::ClearTorque()
{
	pPxActor->clearTorque();
}


void RigidBody::SetPosition(FVector3 position)
{
	pPxActor->setGlobalPose(
		PxTransform(
			physx_cast(position),
			pPxActor->getGlobalPose().q
		)
	);
}

void RigidBody::SetOrientation(FQuaternion orientation)
{
	pPxActor->setGlobalPose(
		PxTransform(
			pPxActor->getGlobalPose().p,
			physx_cast(orientation)
		)
	);
}


void RigidBody::SetGlobalTransform(FTransform transform)
{
	pPxActor->setGlobalPose(
		physx_cast(transform)
	);
}

FTransform RigidBody::GetGlobalTransform() const
{
	FVector3 pos = physx_cast(pPxActor->getGlobalPose().p);
	FQuaternion quat = physx_cast(pPxActor->getGlobalPose().q);

	return FTransform(pos, quat);
}


void RigidBody::SetShape(const PhysicsShape& shape)
{
	const PhysicsMaterial& mat = shape.GetMaterial();

	PxMaterial* pxMaterial = Allocator->fpxService->mPxPhysics->createMaterial(mat.StaticFriction, mat.DynamicFriction, mat.Restitution);
	PxRigidActorExt::createExclusiveShape(*pPxActor, *shape.GetGeometry()->FPXGeometry, *pxMaterial);
}



void RigidBody::SetAngularDamping(float f)
{
	pPxActor->setAngularDamping(f);
}

void RigidBody::SetMass(float f)
{
	pPxActor->setMass(f);
}


/*
* 
*	Static
* 
*/


FTransform StaticRigidBody::GetGlobalTransform() const
{
	return physx_cast(pPxActor->getGlobalPose());
}


void StaticRigidBody::SetGlobalTransform(FTransform transform)
{
	pPxActor->setGlobalPose(
		physx_cast(transform)
	);
}


void StaticRigidBody::SetShape(const PhysicsShape& shape)
{
	const PhysicsMaterial& mat = shape.GetMaterial();

	PxMaterial* pxMaterial = Allocator->fpxService->mPxPhysics->createMaterial(mat.StaticFriction, mat.DynamicFriction, mat.Restitution);
	PxRigidActorExt::createExclusiveShape(*pPxActor, *shape.GetGeometry()->FPXGeometry, *pxMaterial);
}


/*
* 
*	Character
* 
*/



void CharacterBody::Move(FVector3 vec)
{
	auto MoveTimestamp = FTime::GetTimestamp();
	auto MoveTimeDelta = (MoveTimestamp - LastMoveTimestamp).GetSeconds();
	LastMoveTimestamp = MoveTimestamp;

	if (MoveTimeDelta < 0.1f)
	{
		vec += FVector3(0, -15, 0);
		vec *= MoveTimeDelta;

		pPxController->move(physx_cast(vec), 0.01f, MoveTimeDelta, PxControllerFilters());
	}
}


FTransform CharacterBody::GetGlobalTransform() const
{
	PxExtendedVec3 pos = pPxController->getFootPosition();
	return FTransform(FVector3(pos.x, pos.y, pos.z));
}


bool CharacterBody::IsGrounded() const
{
	return true;

}

float CharacterBody::GetHeight() const
{
	return pPxController->getHeight();
}