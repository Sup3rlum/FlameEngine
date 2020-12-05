#pragma once


#include "../../dll/nchfx.h"
#include "../Types/FVector3.h"


EXPORT(class,  BezierCurve)
{
public:
	std::vector<FVector3> _points;

	FVector3 GetPoint(float _t);

};

