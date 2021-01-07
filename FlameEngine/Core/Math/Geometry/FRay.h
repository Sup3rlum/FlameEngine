#pragma once

#include "Core/Common/CoreCommon.h"
#include "../Types/FVector3.h"

#include "FPlane.h"


EXPORT(class, FRay)
{
public:

	FRay(const FVector3& origin, const FVector3& dir) : Origin(origin), Direction(dir) {}
	FRay(const FVector3& dir) : Origin(FVector3(0)), Direction(dir) {}

	bool Intersects(const FPlane& plane) const;

	FVector3 Origin;
	FVector3 Direction;
};

