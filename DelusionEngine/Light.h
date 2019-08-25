#pragma once

#include "nchfx.h"
#include "Color.h"

enum class DELUSION_DLL LightType
{
	AMBIENT = 0,
	DIRECTIONAL = 1,
	POINT = 2,
	SPOT = 3
};

class DELUSION_DLL Light
{
public:
	Vector3 Position;
	Vector3 Direction;
	Color Color;
	float Angle;
	float Intensity;

	LightType Type;
};

