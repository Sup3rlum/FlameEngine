#pragma once

#include "Constants.h"



FLAME_API FORCEINLINE float ToRadians(float degrees)
{
	return PI * degrees / 180.0f;
}

FLAME_API FORCEINLINE float ToDegrees(float radians)
{
	return 180.0f * radians / PI;
}