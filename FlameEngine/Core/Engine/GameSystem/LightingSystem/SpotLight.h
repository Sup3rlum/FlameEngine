#pragma once

#include "Core/Common/CoreCommon.h"
#include "LightBase.h"


EXPORT(class, SpotLight) : Light
{
public:
	SpotLight();

	Texture* mask;
};

