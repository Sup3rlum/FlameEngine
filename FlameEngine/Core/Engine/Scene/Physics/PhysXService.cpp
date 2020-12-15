#include "PhysXService.h"


FLRESULT PhysXService::InitializePhysX()
{
	mPxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	if (mPxFoundation == NULL)
	{
		return FLRESULT::FAIL;
	}
	else
	{
		printf("mPxFoundation loaded\n");
	}

	gTransport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);

	gPvd = PxCreatePvd(*mPxFoundation);
	gPvd->connect(*gTransport, physx::PxPvdInstrumentationFlag::eALL);


	mPxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mPxFoundation, PxTolerancesScale(), false, gPvd);

	
	if (mPxPhysics == NULL)
	{
		return FLRESULT::FAIL;
	}
	else
	{
		printf("mPxPhysics loaded\n");
	}

}	

PxScene* PhysXService::CreateScene()
{
	PxSceneDesc sceneDesc(mPxPhysics->getTolerancesScale());

	sceneDesc.gravity = PxVec3(0.0f, -10.0f, 0.0f);  // acceleration, factor squared

	mPxDispatcher = PxDefaultCpuDispatcherCreate(4);

	/*PxCudaContextManagerDesc cudaContextManagerDesc;
	gCudaContextManager = PxCreateCudaContextManager(*mPxFoundation, cudaContextManagerDesc);*/

	sceneDesc.cpuDispatcher = mPxDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
	sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;
	sceneDesc.sceneQueryUpdateMode = PxSceneQueryUpdateMode::eBUILD_ENABLED_COMMIT_DISABLED;

	return mPxPhysics->createScene(sceneDesc);
}

PxRigidStatic* PhysXService::CreatePlane(PxVec3 n, PxMaterial* m)
{
	return PxCreatePlane(*mPxPhysics, PxPlane(n, 0), *m);
}