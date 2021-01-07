#include "Actor.h"



#include "../Physics/PhysXService.h"
#include "../Physics/PhysXPXCast.h"


class FActor::PhysXRigidDynamicProxy : public PxRigidDynamic
{

};




void FActor::AddForce(FVector3 force)
{
	pPxActor->addForce(physx_cast<FVector3, PxVec3>(force));
}

void FActor::AddTorque(FVector3 torque)
{
	pPxActor->addForce(physx_cast<FVector3, PxVec3>(torque));
}

void FActor::SetLinearVelocity(FVector3 velocity)
{
	pPxActor->setLinearVelocity(physx_cast<FVector3, PxVec3>(velocity));
}
void FActor::SetAngularVelocity(FVector3 velocity)
{
	pPxActor->setAngularVelocity(physx_cast<FVector3, PxVec3>(velocity));

}


void FActor::ClearForce()
{
	pPxActor->clearForce();
}

void FActor::ClearTorque()
{
	pPxActor->clearTorque();
}


void FActor::SetPosition(FVector3 position)
{
	pPxActor->setGlobalPose(
		PxTransform(
			physx_cast<FVector3, PxVec3>(position), 
			pPxActor->getGlobalPose().q
		)
	);
}

void FActor::SetOrientation(FQuaternion orientation)
{
	pPxActor->setGlobalPose(
		PxTransform(		
			pPxActor->getGlobalPose().p,
			physx_cast<FQuaternion, PxQuat>(orientation)
		)
	);
}


void FActor::SetGlobalTransform(FTransform transform)
{
	pPxActor->setGlobalPose(
		physx_cast<FTransform, PxTransform>(transform)
	);
}

void FActor::SetRelativeTransform(FActor& relative, FTransform transform)
{

}