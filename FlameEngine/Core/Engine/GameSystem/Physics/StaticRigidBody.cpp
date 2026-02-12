#include "StaticRigidBody.h"

#include "PX/FPXActorProxy.h"
#include "PX/FPXAllocator.h"
#include "PX/FPXService.h"
#include "PX/FPXScene.h"
#include "PhysicsGeometryProxy.h"


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


void StaticRigidBody::SetShape(const PhysShape& shape)
{
	const PhysicsMaterial& mat = shape.GetMaterial();

	PxMaterial* pxMaterial = Allocator->fpxService->mPxPhysics->createMaterial(mat.StaticFriction, mat.DynamicFriction, mat.Restitution);
	PxRigidActorExt::createExclusiveShape(*pPxActor, *shape.GetGeometry()->FPXGeometry, *pxMaterial);

}
void StaticRigidBody::RemoveShapes()
{
	int numShapes = pPxActor->getNbShapes();

	if (numShapes)
	{
		physx::PxShape** pxShapes = new physx::PxShape * [numShapes];
		pPxActor->getShapes(pxShapes, numShapes, 0);

		for (int i = 0; i < numShapes; i++)
		{
			pPxActor->detachShape(*pxShapes[i]);
		}
		delete[] pxShapes;
	}
}
void StaticRigidBody::Release()
{
	pPxActor->release();
}