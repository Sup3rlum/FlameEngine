#pragma once

#include "Constants.h"



FLAME_DLL constexpr float ToRadians(float degrees)
{
	return PI * degrees / 180.0f;
}

FLAME_DLL constexpr float ToDegrees(float radians)
{
	return 180.0f * radians / PI;
}