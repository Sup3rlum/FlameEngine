#pragma once

#include "../dll/nchfx.h"
#include "../Mathematics/Module.h"


EXPORT_ENUM IntersectionType
{
	Contains = 0,
	Intersects = 1,
	Disjoint = 2
};

EXPORT_CLASS Plane
{
public:
	fVector4 _internal;
	Plane();
	Plane(fVector3 _v1, fVector3 _v2, fVector3 _v3);
	Plane(fVector3 _normal, float _det);
	Plane(fVector4 _v);

	float Distance(fVector3 point);
	float DistanceSigned(fVector3 point);

	bool Contains(fVector3 point);
	fVector3 Normal();
};

