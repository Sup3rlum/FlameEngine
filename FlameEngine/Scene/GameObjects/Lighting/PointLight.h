#pragma once

#include "../../../dll/nchfx.h"

#include "Light.h"

EXPORT(class, PointLight) : public Light 
{
public:
	FVector3 Position;
	FVector3 Parameters;

	PointLight(FVector3 position, Color color, FVector3 parameters) : Position(position), Parameters(parameters) 
	{
		LightColor = color;
	}
};

