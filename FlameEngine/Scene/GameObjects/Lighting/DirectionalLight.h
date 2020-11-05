#pragma once


#include "Light.h"

EXPORT_STRUCT DirectionalLight : public Light
{
public:
	fVector3 Direction;
	float Intensity;

	DirectionalLight(fVector3 _direction, Color _color, float _intensity);
};