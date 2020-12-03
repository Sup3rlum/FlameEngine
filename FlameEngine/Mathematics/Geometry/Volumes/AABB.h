#pragma once

#include "../../../dll/nchfx.h"
#include "../../Types/fVector4.h"

EXPORT(class, AABB)
{
public:
	fVector3 minPoint;
	fVector3 maxPoint;

	AABB();
	AABB(fVector3 min, fVector3 max);

	void Enclose(fVector3 point);

	float LengthX();
	float LengthY();
	float LengthZ();

	fVector3 Center();

	float Volume();

	void GetCorners(fVector3(&corners)[8]);

	static AABB FromPointArray(fVector3* points, int numPoints);

	template<size_t _Size>
	static AABB FromPointArray(fVector3 (&points)[_Size]);

	void SetDegenerate();
};

