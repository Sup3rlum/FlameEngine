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

void AABB::GetCorners(fVector3(&corners)[8])
{
	fVector3 ndc_corners[8] =
	{
		fVector3(1.0f, -1.0f, -1.0f),	 // llb
		fVector3(-1.0f, -1.0f, -1.0f), // lrb
		fVector3(-1.0f, 1.0f, -1.0f),  // urb
		fVector3(1.0f, 1.0f, -1.0f),	 // ulb


		fVector3(1.0f, -1.0f, 1.0f),   // llf
		fVector3(-1.0f, -1.0f, 1.0f),  // lrf
		fVector3(-1.0f, 1.0f, 1.0f),   // urf
		fVector3(1.0f, 1.0f, 1.0f)	 // ulf

	};
	for (int i = 0; i < 8; i++)
	{
		fVector3 _one = ndc_corners[i] * 0.5f + 0.5f;
		corners[i] = _one * maxPoint + (fVector3(1) - _one) * minPoint;
	}
}

void AABB::Enclose(fVector3 point)
{
	minPoint = fVector3::Min(minPoint, point);
	maxPoint = fVector3::Max(maxPoint, point);
}

fVector3 AABB::Center()
{
	return (maxPoint - minPoint) / 2.0f + minPoint;
}

float AABB::LengthX()
{
	return maxPoint.x - minPoint.x;
}
float AABB::LengthY()
{
	return maxPoint.y - minPoint.y;
}
float AABB::LengthZ()
{
	return maxPoint.z - minPoint.z;
}

float AABB::Volume()
{
	return LengthX() * LengthY() * LengthZ();
}

AABB AABB::FromPointArray(fVector3* points, int numPoints)
{

	AABB aabb;
	aabb.SetDegenerate();
	for (int i = 0; i < numPoints; i++)
	{
		aabb.Enclose(points[i]);
	}
	return aabb;
}

template<size_t _Size>
AABB FromPointArray(fVector3(&points)[_Size])
{

	return FromPointArray(&points[0], _Size);
}

