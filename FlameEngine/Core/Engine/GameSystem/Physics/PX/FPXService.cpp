#include "FPXService.h"
#include "FPXAllocator.h"
#include "FPXActorProxy.h"
#include "FPXScene.h"


PxDefaultErrorCallback gErrorCallback;
PxDefaultAllocator gAllocator;

// Visual Debugging
PxPvd* gPvd;
PxPvdTransport* gTransport;



FPXService::FPXService() :
	mPxDispatcher(NULL),
	mPxCooking(NULL)
{
	mPxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	if (mPxFoundation == NULL)
	{
		// TODO: Error PhysX Foundation
	}


	gTransport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);

	gPvd = PxCreatePvd(*mPxFoundation);
	gPvd->connect(*gTransport, physx::PxPvdInstrumentationFlag::eALL);


	mPxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mPxFoundation, PxTolerancesScale(), false, gPvd);
	mPxCooking = PxCreateCooking(PX_PHYSICS_VERSION, * mPxFoundation, PxCookingParams(PxTolerancesScale()));

	if (mPxPhysics == NULL)
	{
		// TODO: Error PhysX
	}
	if (mPxCooking == NULL)
	{
		// TODO: Error PhysX Cooking
	}

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

