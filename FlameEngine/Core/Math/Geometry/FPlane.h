#pragma once

#include "Core/Common/CoreCommon.h"
#include "../Types/Vector.h"

EXPORT(class,  FPlane)
{
public:
	FPlane() : value(FVector4(0)) {}
	FPlane(const FVector3& normal, float distance);
	FPlane(const FVector4& vec) : value(vec) {}

	FVector3 GetNormal();

	bool Contains(const FVector3& point);


private:
	FVector4 value;
};

