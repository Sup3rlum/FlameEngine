#pragma once

#include "../../../dll/nchfx.h"

#include "Light.h"

EXPORT(class, PointLight) : public Light 
{
public:
	FVector3 Position;
	float Intensity;
	float Radius;

	PointLight(FVector3 position, Color color, float intensity, float radius) : Position(position), Intensity(intensity), Radius(radius)
	{
		LightColor = color;
	}
};

