#pragma once


#include "ITimeDimensional.h"


EXPORT(struct, FScalar)
{
	float data;
public:

	FScalar(const float& f)
	{
		data = f;
	}

	FScalar operator=(float f)
	{
		data = f;
	}



};