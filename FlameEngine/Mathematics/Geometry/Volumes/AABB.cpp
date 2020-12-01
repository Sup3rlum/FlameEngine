#include "AABB.h"

AABB::AABB()
{

}

AABB::AABB(fVector3 min, fVector3 max)
{
	minPoint = min;
	maxPoint = max;
}

void AABB::SetDegenerate()
{
	minPoint = fVector3(std::numeric_limits<float>::infinity());
	maxPoint = fVector3(-std::numeric_limits<float>::infinity());
}



AABB AABB::FromPointArray(fVector3* points, int numPoints)
{

	AABB aabb;
	aabb.SetDegenerate();
	for (int i = 0; i < numPoints; i++)
	{
		aabb.Enclose(points[i]);
	}
}

template<size_t _Size>
static AABB FromPointArray(fVector3(&points)[_Size])
{

	return FromPointArray(&points[0], _Size);
}