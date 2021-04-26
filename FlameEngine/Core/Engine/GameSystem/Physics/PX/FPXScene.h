#pragma once


#include "../PhysicsScene.h"
#include "PhysXFrameworkPX_Includes.h"

class FPXScene : public PhysicsScene
{

	friend class FPXService;
	friend class FPXAllocator;

private:
	physx::PxScene* sceneHandle;

	FPXScene(physx::PxScene* pxScene) :
		sceneHandle(pxScene)
	{}
	
public:
	void Step(float dt);
};