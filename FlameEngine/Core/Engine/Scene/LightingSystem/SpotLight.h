#pragma once

#include "../../../dll/nchfx.h"
#include "Light.h"


EXPORT(class, SpotLight) : Light
{
public:
	SpotLight();

	Texture* mask;
};

