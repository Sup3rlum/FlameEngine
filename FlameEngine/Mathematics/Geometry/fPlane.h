#pragma once

#include "../../dll/nchfx.h"
#include "../Types/fVector4.h"

EXPORT(class,  fPlane)
{
public:
	fPlane() : value(fVector4(0)) {}
	fPlane(fVector3 normal, float distance);
	fPlane(fVector4 vec) : value(vec) {}


	bool Contains(fVector3 point);


private:
	fVector4 value;
};

