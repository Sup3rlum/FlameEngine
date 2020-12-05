#include "AABB.h"

AABB::AABB()
{

}

AABB::AABB(FVector3 min, FVector3 max)
{
	minPoint = min;
	maxPoint = max;
}

void AABB::SetDegenerate()
{
	minPoint = FVector3(std::numeric_limits<float>::infinity());
	maxPoint = FVector3(-std::numeric_limits<float>::infinity());
}

void AABB::GetCorners(FVector3(&corners)[8])
{
	FVector3 ndc_corners[8] =
	{
		FVector3(1.0f, -1.0f, -1.0f),	 // llb
		FVector3(-1.0f, -1.0f, -1.0f), // lrb
		FVector3(-1.0f, 1.0f, -1.0f),  // urb
		FVector3(1.0f, 1.0f, -1.0f),	 // ulb


		FVector3(1.0f, -1.0f, 1.0f),   // llf
		FVector3(-1.0f, -1.0f, 1.0f),  // lrf
		FVector3(-1.0f, 1.0f, 1.0f),   // urf
		FVector3(1.0f, 1.0f, 1.0f)	 // ulf

	};
	for (int i = 0; i < 8; i++)
	{
		FVector3 _one = ndc_corners[i] * 0.5f + 0.5f;
		corners[i] = _one * maxPoint + (FVector3(1) - _one) * minPoint;
	}
}

void AABB::Enclose(FVector3 point)
{
	minPoint = FVector3::Min(minPoint, point);
	maxPoint = FVector3::Max(maxPoint, point);
}

FVector3 AABB::Center()
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

AABB AABB::FromPointArray(FVector3* points, int numPoints)
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
AABB FromPointArray(FVector3(&points)[_Size])
{

	return FromPointArray(&points[0], _Size);
}

