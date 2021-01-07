#pragma once

#include "Core/Common/CoreCommon.h"

#include <PxFoundation.h>
#include <PxPhysics.h>
#include <PxDefaultAllocator.h>
#include <PxDefaultErrorCallback.h>
#include <PxDefaultCpuDispatcher.h>
#include <PxPhysicsVersion.h>
#include <PxScene.h>
#include <PxShape.h>
#include <PxActor.h>
#include <PxMaterial.h>
#include <PxRigidBody.h>
#include <PxRigidDynamic.h>
#include <PxRigidStatic.h>
#include <PxRigidActor.h>

#include <pvd/PxPvd.h>
#include <pvd/PxPvdTransport.h>
#include <PxDefaultSimulationFilterShader.h>

#include <PxRigidActorExt.h>
#include <PxRigidBodyExt.h>

#include <PxSimpleFactory.h>


using namespace physx;


EXPORT(class,  PhysXService)
{
public:

	void InitializePhysX();
	PxScene* CreateScene();


	PxFoundation* mPxFoundation;

	PxDefaultErrorCallback gErrorCallback;
	PxDefaultAllocator gAllocator;

	PxPhysics* mPxPhysics;

	PxPvd* gPvd;
	PxPvdTransport* gTransport;

	PxDefaultCpuDispatcher* mPxDispatcher;


	PxRigidStatic* CreatePlane(PxVec3, PxMaterial*);

};

