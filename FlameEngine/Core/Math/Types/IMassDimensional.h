#pragma once

#include "Core/Common/CoreCommon.h"

template<int Dim>
EXPORT(class, IMassDimensional)
{
	int GetMDimension() const { return Dim; }
};