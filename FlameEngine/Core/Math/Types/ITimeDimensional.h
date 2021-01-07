#pragma once

#include "Core/Common/CoreBase.h"


/*
* 
*		Provides templating for physical quantities that have a time dimenstion component of s^-Dim
*		e.g. Velocity is measured in distance per second -> us^-1 therefore it has an inverse time dim of 1
*		e.g. Displacement has no time component so its inverse time dim is 0
* 
*		In physics, for example adding an acceleration to a velocity doesn't make sense as they have different units,
*		even though they have the same spatial dimensionality.
* 
*/

template<int Dim>
EXPORT(class, ITimeDimensional)
{
	int GetTDimension() const { return Dim; }

	static bool CompareDim(ITimeDimensional& left, ITimeDimensional& right)
	{
		return left.GetTDimension() == right.GetTDimension();
	};
};