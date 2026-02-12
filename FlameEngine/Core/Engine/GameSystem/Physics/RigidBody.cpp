#include "RigidBody.h"

#include "PX/FPXActorProxy.h"
#include "PX/FPXAllocator.h"
#include "PX/FPXService.h"
#include "PX/FPXScene.h"
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


void RigidBody::SetShape(const PhysShape& shape)
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


void RigidBody::Release()
{
	pPxActor->release();
}