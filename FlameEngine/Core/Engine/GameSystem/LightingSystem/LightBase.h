#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Engine/Renderer/Common/Color.h"
#include "Core/Math/Module.h"
#include "../CameraSystem/Camera.h"

#define MAX_DIRECTIONAL_LIGHTS 5
#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 16
#define MAX_AMBIENT_LIGHTS 5


EXPORT(struct,  Light)
{
public:
	Color LightColor;
	float Intensity;
	Camera camera;
	//LensFlare* Flare;

	virtual Camera* LightCamera() = 0;
	virtual void Update();
	virtual void DrawFlare();

	virtual void CreateFlares();

	Light(Color color);
	Light();
};

