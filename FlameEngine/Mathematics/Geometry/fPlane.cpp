#include "FPlane.h"

FPlane::FPlane(FVector3 norm, float d)
{
	value = FVector4(FVector3::Normalize(norm), -d);
}

bool FPlane::Contains(FVector3 point)
{
	return FVector4::Dot(value, FVector4(point, 1)) == 0;
}

FVector3 FPlane::GetNormal()
{
	return FVector3::Normalize(value.xyz);
}