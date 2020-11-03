#pragma once

#include "../../dll/nchfx.h"
#include "../../Graphics/Color.h"

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
	fVector3 Position;
	fVector3 Direction;
	Color Color;
	float Angle;
	float Intensity;

	LightType Type;
};

