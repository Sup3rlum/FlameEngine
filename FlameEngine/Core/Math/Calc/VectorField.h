#pragma once


#include "../Types/Vector.h"
#include "../Geometry/Volumes/AABB.h"

struct VectorField
{
	typedef FDelegate<FVector3(const FVector3&)> VectorFunction;
	enum class CoordinateMode
	{
		Relative,
		Absolute
	};

	CoordinateMode coordinateMode;
	VectorFunction vectorFunction;
	AABB Volume;

	VectorField(AABB volume, VectorFunction vectorFunc = [](const FVector3& vec) -> FVector3 { return 0; }, CoordinateMode coordinateMode = CoordinateMode::Relative) :
		coordinateMode(coordinateMode),
		vectorFunction(vectorFunc),
		Volume(volume)
	{

	}

	FVector3 Calculate(const FVector3& vec) const
	{

		if (coordinateMode == CoordinateMode::Absolute)
		{
			return vectorFunction(vec - Volume.minPoint);
		}
		else
		{
			return vectorFunction(vec);
		}
	}
};