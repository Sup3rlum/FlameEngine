#pragma once

#include "Core/Common/CoreCommon.h"
#include "../Types/FVector3.h"

#include "FPlane.h"


EXPORT(class, FRay)
{
public:

	FRay(FVector3 origin, FVector3 dir) : Origin(origin), Direction(dir) {}
	FRay(FVector3 dir) : Origin(FVector3(0)), Direction(dir) {}

	bool Intersects(FPlane plane);

	FVector3 Origin;
	FVector3 Direction;
};

