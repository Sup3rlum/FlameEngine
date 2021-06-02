#pragma once


#include "../PhysicsScene.h"
#include "PhysXFrameworkPX_Includes.h"

class FPXScene : public PhysicsScene
{

	friend class FPXService;
	friend class FPXAllocator;

private:
	physx::PxScene* sceneHandle;
	physx::PxControllerManager* mPxControllerManager;

	float StepAccumulator;
	float StepSize;

	FPXScene(physx::PxScene* pxScene) :
		sceneHandle(pxScene),
		StepAccumulator(0),
		StepSize(1.0f/128.0f)
	{
		
		mPxControllerManager = PxCreateControllerManager(*sceneHandle);
	}
	
public:
	void Step(float dt);
};