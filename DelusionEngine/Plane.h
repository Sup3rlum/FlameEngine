#pragma once

#include "nchfx.h"

using namespace glm;

enum class DELUSION_DLL IntersectionType
{
	Contains = 0,
	Intersects = 1,
	Disjoint = 2
};

class DELUSION_DLL Plane
{
public:
	vec4 _internal;
	Plane();
	Plane(vec3 _v1, vec3 _v2, vec3 _v3);
	Plane(vec3 _normal, float _det);
	Plane(vec4 _v);

	float Distance(vec3 point);
	float DistanceSigned(vec3 point);

	bool Contains(vec3 point);
};

