#pragma once

#include "Core/Common/CoreCommon.h"
#include "../Types/FVector4.h"

EXPORT(class,  FPlane)
{
public:
	FPlane() : value(FVector4(0)) {}
	FPlane(FVector3 normal, float distance);
	FPlane(FVector4 vec) : value(vec) {}

	FVector3 GetNormal();

	bool Contains(FVector3 point);


private:
	FVector4 value;
};

