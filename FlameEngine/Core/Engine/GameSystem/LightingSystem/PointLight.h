#pragma once

#include "Core/Common/CoreCommon.h"
#include "LightBase.h"

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

