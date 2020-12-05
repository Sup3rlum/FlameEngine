#include "fPlane.h"

fPlane::fPlane(FVector3 norm, float d)
{
	value = FVector4(FVector3::Normalize(norm), -d);
}

bool fPlane::Contains(FVector3 point)
{
	return FVector4::Dot(value, FVector4(point, 1)) == 0;
}