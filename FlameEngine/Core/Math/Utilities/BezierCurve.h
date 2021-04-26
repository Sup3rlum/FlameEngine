#pragma once


#include "Core/Common/CoreCommon.h"
#include "../Types/Vector.h"


EXPORT(class,  BezierCurve)
{
public:
	FArray<FVector3> _points;

	FVector3 GetPoint(float _t);

};

