#pragma once


#include "../../dll/nchfx.h"
#include "../Types/fVector3.h"


EXPORT(class,  BezierCurve)
{
public:
	std::vector<fVector3> _points;

	fVector3 GetPoint(float _t);

};

