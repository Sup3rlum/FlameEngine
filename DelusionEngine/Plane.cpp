#include "Plane.h"

Plane::Plane()
{
	_internal = vec4(0);
}

Plane::Plane(vec3 n, float d)
{
	float l = length(n);

	_internal = vec4(n, d) / l;
}
Plane::Plane(vec4 v)
{
	_internal = v;
}
Plane::Plane(vec3 _v1, vec3 _v2, vec3 _v3)
{
	vec3 _n = normalize(cross(_v1 - _v2,_v3 - _v2));
	float d = -dot(_n, _v1);

	_internal = vec4(_n, d);
}

float Plane::Distance(vec3 p)
{
	return abs(DistanceSigned(p));
}
float Plane::DistanceSigned(vec3 p)
{
	return dot(vec4(p, 1.0f), _internal);
}


bool Plane::Contains(vec3 p)
{
	return DistanceSigned(p) == 0;
}
