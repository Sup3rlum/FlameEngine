#pragma once


#include "Light.h"

EXPORT(struct,  AmbientLight) : public Light
{
public:
	float Intensity;
};