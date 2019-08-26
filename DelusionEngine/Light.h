#pragma once

#include "nchfx.h"
#include "Color.h"

EXPORT_ENUM LightType
{
	AMBIENT = 0,
	DIRECTIONAL = 1,
	POINT = 2,
	SPOT = 3
};

EXPORT_STRUCT Light
{
public:
	Vector3 Position;
	Vector3 Direction;
	Color Color;
	float Angle;
	float Intensity;

	LightType Type;
};

