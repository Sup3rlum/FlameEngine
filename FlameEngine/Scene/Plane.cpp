#include "Plane.h"

Plane::Plane()
{
	_internal = fVector4(0);
}

Plane::Plane(fVector3 n, float d)
{
	float l = n.Length();

	_internal = fVector4(n / l, -d);
}
Plane::Plane(fVector4 v)
{
	_internal = v;
}
Plane::Plane(fVector3 _v1, fVector3 _v2, fVector3 _v3)
{
	fVector3 _n = fVector3::Normalize(fVector3::Cross(_v1 - _v2,_v3 - _v2));
	float d = -fVector3::Dot(_n, _v1);

	_internal = fVector4(_n, d);
}

float Plane::Distance(fVector3 p)
{
	return abs(DistanceSigned(p));
}
float Plane::DistanceSigned(fVector3 p)
{
	return fVector4::Dot(fVector4(p, 1.0f), _internal);
}


bool Plane::Contains(fVector3 p)
{
	return DistanceSigned(p) == 0;
}

fVector3 Plane::Normal()
{
	return fVector3(_internal.x, _internal.y, _internal.z);
}