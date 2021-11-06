#pragma once

#include "Core/Common/CoreCommon.h"
#include "../Types/Vector.h"

#include "FPlane.h"


struct FRay
{
public:

	FRay(const FVector3& origin, const FVector3& dir) : Origin(origin), Direction(dir) {}
	FRay(const FVector3& dir) : Origin(FVector3(0)), Direction(dir) {}


	FVector3 Origin;
	FVector3 Direction;
};

