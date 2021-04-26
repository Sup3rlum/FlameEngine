#include "FPXService.h"
#include "FPXAllocator.h"
#include "FPXShape.h"
#include "FPXActorProxy.h"
#include "FPXScene.h"

PhysicsShape* FPXAllocator::CreateShape(PhysicsMaterial material, BoxGeometry geom)
{
	return new FPXShape(fpxService->mPxPhysics, material, geom);
}

PhysicsShape* FPXAllocator::CreateShape(PhysicsMaterial material, SphereGeometry geom)
{
	return new FPXShape(fpxService->mPxPhysics, material, geom);
}

PhysicsShape* FPXAllocator::CreateShape(PhysicsMaterial material, CapsuleGeometry geom)
{
	return new FPXShape(fpxService->mPxPhysics, material, geom);
}
PhysicsShape* FPXAllocator::CreateShape(PhysicsMaterial material, PlaneGeometry geom)
{
	return new FPXShape(fpxService->mPxPhysics, material, geom);
}


DynamicPhysicsComponent FPXAllocator::CreateDynamic(FTransform transform)
{

	//PxTransform tr = physx_cast(transform);


	PxVec3 vec = physx_cast(transform.Position);
	PxQuat quat = physx_cast(transform.Orientation);

	PxTransform tr(vec, quat);

	auto bodyPtr = fpxService->mPxPhysics->createRigidDynamic(tr);

	fpxScene->sceneHandle->addActor(*bodyPtr);

	return DynamicPhysicsComponent(static_cast<DynamicPhysicsComponent::PhysXActorProxy*>(bodyPtr));
}


StaticPhysicsComponent FPXAllocator::CreateStatic(FTransform transform)
{

	PxVec3 vec = physx_cast(transform.Position);
	PxQuat quat = physx_cast(transform.Orientation);

	PxTransform tr(vec, quat);


	auto bodyPtr = fpxService->mPxPhysics->createRigidStatic(tr);

	fpxScene->sceneHandle->addActor(*bodyPtr);

	return StaticPhysicsComponent(static_cast<StaticPhysicsComponent::PhysXActorProxy*>(bodyPtr));
}



// SERVICE


FPXService::FPXService() :
	mPxDispatcher(NULL)
{
	mPxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	if (mPxFoundation == NULL)
	{
		std::cout << "Big problem mPxFoundation" << std::endl;
	}


	gTransport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);

	gPvd = PxCreatePvd(*mPxFoundation);
	gPvd->connect(*gTransport, physx::PxPvdInstrumentationFlag::eALL);


	mPxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mPxFoundation, PxTolerancesScale(), false, gPvd);

	if (mPxPhysics == NULL)
	{
		std::cout << "Big problem mPxPhysics" << std::endl;
	}


	PxQuat pxquat(0.3f, PxVec3(0.707f, 0.707f, 0));

	FQuaternion fquat = FQuaternion::FromAxisAngle(0.3f, FVector3(0.707f, 0.707f, 0));

	std::cout << "PxQuat: X: " << pxquat.x << " Y: " << pxquat.y << " Z: " << pxquat.z << " W: " << pxquat.w << std::endl;
	std::cout << "FQuat: X: " << fquat.x << " Y: " << fquat.y << " Z: " << fquat.z << " W: " << fquat.w << std::endl;

	fquat = physx_cast(pxquat);

	std::cout << "FQuat: X: " << fquat.x << " Y: " << fquat.y << " Z: " << fquat.z << " W: " << fquat.w << std::endl;


}


PhysicsScene* FPXService::CreateScene(FVector3 gravity)
{

	mPxDispatcher = PxDefaultCpuDispatcherCreate(4);

	PxSceneDesc sceneDesc(mPxPhysics->getTolerancesScale());

	sceneDesc.cpuDispatcher = mPxDispatcher;
	sceneDesc.gravity = physx_cast(gravity);
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
	sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;
	sceneDesc.sceneQueryUpdateMode = PxSceneQueryUpdateMode::eBUILD_ENABLED_COMMIT_DISABLED;


	return new FPXScene(mPxPhysics->createScene(sceneDesc));
}




// SCENE



void FPXScene::Step(float dt)
{
	sceneHandle->simulate(dt);
	sceneHandle->fetchResults(true);
}