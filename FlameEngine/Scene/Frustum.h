#pragma once

#include "../dll/nchfx.h"
#include "BoundingBox.h"
#include "Plane.h"

EXPORT_CLASS Frustum
{
public:
	Frustum(fMatrix4 PV);

	bool Contains(BoundingBox _box);
	bool Contains(fVector3 _p);

	IntersectionType Intersects(BoundingBox _box);

	//Plane zNear, zFar, Left, Right, Top, Bottom;
	Plane planes[6];
};

