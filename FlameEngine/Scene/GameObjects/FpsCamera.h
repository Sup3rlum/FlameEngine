#pragma once

#include "Camera.h"


EXPORT_CLASS FpsCamera : public Camera
{

public:
	FpsCamera(Context* _cont);
	~FpsCamera();

	float Fov = 45.0f;

	float horizontalAngle;
	float verticalAngle;

	void Update();

	float mouseSpeed = 0.3f;
	float flySpeed = 10.0f;

	Context* _currentContext;
};
