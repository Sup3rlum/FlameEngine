#pragma once

#include "../../../dll/nchfx.h"
#include "../../../Graphics/Common/Color.h"
#include "../../../Mathematics/Module.h"

EXPORT(struct,  Light)
{
public:
	Color LightColor;

	fMatrix4 View;
	fMatrix4 Projection;

	Light(Color _color);
	Light();
};

