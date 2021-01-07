#include "BezierCurve.h"


FVector3 BezierCurve::GetPoint(float _t)
{/*
	FVector3* _subPoints = new FVector3[_points.size()];

	memcpy(_subPoints, &_points[0], sizeof(_points));

	for (int i = _points.size() - 1; i > 0; i--)
	{
		for (int j = 0; j < i; j++)
		{
			_subPoints[j] = FVector3::Lerp(_subPoints[j], _subPoints[j + 1], _t);
		}
	}*/

	return 0;// _subPoints[0];

}