#pragma once

#include "../../../dll/nchfx.h"
#include "../../../Graphics/Common/Color.h"
#include "../../../Mathematics/Module.h"
#include "../Camera.h"
#include "../UpdatableObject.h"
#include "../Visual/LensFlare.h"


#define MAX_DIRECTIONAL_LIGHTS 5
#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 16
#define MAX_AMBIENT_LIGHTS 5


EXPORT(struct,  Light)
{
public:
	Color LightColor;
	Camera _cam;
	LensFlare* Flare;

	virtual Camera* LightCamera();
	virtual void Update();
	virtual void DrawFlare();

	virtual void CreateFlares();

	Light(Color _color);
	Light();
};

