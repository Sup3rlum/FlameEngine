#pragma once

#include "../dll/nchfx.h"

using namespace glm;

EXPORT_ENUM IntersectionType
{
	Contains = 0,
	Intersects = 1,
	Disjoint = 2
};

EXPORT_CLASS Plane
{
public:
	Vector4 _internal;
	Plane();
	Plane(Vector3 _v1, Vector3 _v2, Vector3 _v3);
	Plane(Vector3 _normal, float _det);
	Plane(Vector4 _v);

	float Distance(Vector3 point);
	float DistanceSigned(Vector3 point);

	bool Contains(Vector3 point);
};

