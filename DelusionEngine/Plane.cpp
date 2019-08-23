#include "Plane.h"

Plane::Plane()
{
	_internal = Vector4(0);
}

Plane::Plane(Vector3 n, float d)
{
	float l = length(n);

	_internal = Vector4(n, d) / l;
}
Plane::Plane(Vector4 v)
{
	_internal = v;
}
Plane::Plane(Vector3 _v1, Vector3 _v2, Vector3 _v3)
{
	Vector3 _n = normalize(cross(_v1 - _v2,_v3 - _v2));
	float d = -dot(_n, _v1);

	_internal = Vector4(_n, d);
}

float Plane::Distance(Vector3 p)
{
	return abs(DistanceSigned(p));
}
float Plane::DistanceSigned(Vector3 p)
{
	return dot(Vector4(p, 1.0f), _internal);
}


bool Plane::Contains(Vector3 p)
{
	return DistanceSigned(p) == 0;
}
