#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"

#include "PhysXFrameworkPX_Includes.h"
#include "../PhysicsService.h"

using namespace physx;


class FPXService : public PhysicsService
{
public:


	FPXService();

	PhysicsScene* CreateScene(FVector3 gravity);
	

	PxFoundation* mPxFoundation;

	PxDefaultErrorCallback gErrorCallback;
	PxDefaultAllocator gAllocator;

	PxPhysics* mPxPhysics;

	PxPvd* gPvd;
	PxPvdTransport* gTransport;

	PxDefaultCpuDispatcher* mPxDispatcher;


};

