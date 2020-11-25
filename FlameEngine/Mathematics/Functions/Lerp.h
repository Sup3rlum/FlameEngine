#pragma once

#include "../../dll/nchfx.h"


template<typename T>
FLAME_DLL inline T Lerp(T a, T b, float t)
{
	return a + t * (b - a);
}

