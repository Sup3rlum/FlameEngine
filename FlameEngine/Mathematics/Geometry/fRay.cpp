#include "FRay.h"


bool FRay::Intersects(FPlane plane)
{
	return (FVector3::Dot(Direction, plane.GetNormal()) > 0.0001f);
}