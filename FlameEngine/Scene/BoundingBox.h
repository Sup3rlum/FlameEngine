#pragma once

#include "../dll/nchfx.h"
#include "../Mathematics/Module.h"


EXPORT_CLASS BoundingBox
{
public:
	fVector3 Min;
	fVector3 Max;
	fMatrix4 Rotation;

	BoundingBox();
	BoundingBox(fVector3 min, fVector3 max);
	BoundingBox(fVector3 min, fVector3 max, fMatrix4 rot);

	fVector3* GetCorners();

	bool Contains(fVector3 p);

};

