#pragma once

#include "../../../dll/nchfx.h"
#include "../../../Graphics/Common/Color.h"
#include "../../../Mathematics/Module.h"
#include "../Camera.h"
#include "../UpdatableObject.h"

EXPORT(struct,  Light)
{
public:
	Color LightColor;
	Camera _cam;

	virtual Camera* LightCamera();
	virtual void Update();

	Light(Color _color);
	Light();
};

