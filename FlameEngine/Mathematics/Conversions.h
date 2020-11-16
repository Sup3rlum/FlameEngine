#pragma once

#include "Constants.h"



constexpr float ToRadians(float degrees)
{
	return PI * degrees / 180.0f;
}

constexpr float ToDegrees(float radians)
{
	return 180.0f * radians / PI;
}