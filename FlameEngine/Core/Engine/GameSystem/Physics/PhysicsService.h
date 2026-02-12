#pragma once


#include "Core/Common/CoreCommon.h"


#include "PhysicsScene.h"


struct PhysicsDescription
{
	FVector3 Gravity;
};


class PhysicsService
{
public:
	virtual PhysicsScene* CreateScene(FVector3 gravity) = 0;
};



