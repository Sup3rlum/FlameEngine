#pragma once


#include "Light.h"
#include "../Camera.h"

EXPORT_STRUCT DirectionalLight : public Light
{
public:
	fVector3 Direction;
	fVector3 _positionInternal;
	float Intensity;

	Camera* LightCamera();
	
	Camera _cam;

	DirectionalLight(fVector3 _direction, Color _color, float _intensity);
};