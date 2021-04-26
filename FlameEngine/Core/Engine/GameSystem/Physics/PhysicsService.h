#pragma once


#include "Core/Common/CoreCommon.h"


#include "PhysicsScene.h"

class PhysicsService
{
public:
	virtual PhysicsScene* CreateScene(FVector3 gravity) = 0;
};