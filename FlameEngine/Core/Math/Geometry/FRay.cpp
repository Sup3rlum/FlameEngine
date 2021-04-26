#include "FRay.h"


bool FRay::Intersects(const FPlane& plane) const
{
	return true;// (FVector3::Dot(Direction, plane.GetNormal()) > 0.0001f);
}