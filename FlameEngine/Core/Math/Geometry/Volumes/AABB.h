#pragma once

#include "Core/Common/CoreCommon.h"
#include "../../Types/FVector4.h"

EXPORT(class, AABB)
{
public:
	FVector3 minPoint;
	FVector3 maxPoint;

	AABB();
	AABB(FVector3 min, FVector3 max);

	void Enclose(FVector3 point);

	float LengthX();
	float LengthY();
	float LengthZ();

	FVector3 Center();

	float Volume();

	void GetCorners(FVector3(&corners)[8]);

	static AABB FromPointArray(FVector3* points, int numPoints);

	template<size_t _Size>
	static AABB FromPointArray(FVector3 (&points)[_Size]);

	void SetDegenerate();
};

