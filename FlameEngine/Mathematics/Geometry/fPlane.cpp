#include "fPlane.h"

fPlane::fPlane(fVector3 norm, float d)
{
	value = fVector4(fVector3::Normalize(norm), -d);
}

bool fPlane::Contains(fVector3 point)
{
	return fVector4::Dot(value, fVector4(point, 1)) == 0;
}