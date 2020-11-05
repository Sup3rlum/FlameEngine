#pragma once


#include "Light.h"

EXPORT_STRUCT AmbientLight : public Light
{
public:
	float Intensity;
};