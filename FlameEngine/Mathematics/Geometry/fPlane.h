#pragma once

#include "../../dll/nchfx.h"
#include "../Types/FVector4.h"

EXPORT(class,  fPlane)
{
public:
	fPlane() : value(FVector4(0)) {}
	fPlane(FVector3 normal, float distance);
	fPlane(FVector4 vec) : value(vec) {}


	bool Contains(FVector3 point);


private:
	FVector4 value;
};

