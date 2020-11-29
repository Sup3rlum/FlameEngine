#pragma once

#include "Camera.h"


EXPORT(class,  FpsCamera) : public Camera
{

public:
	FpsCamera(Context* _cont);
	~FpsCamera();

	float Fov = 45.0f;

	float horizontalAngle;
	float verticalAngle;

	void Update();

	float mouseSpeed = 0.3f;
	float flySpeed = 50.0f;


	void SetCursorLocked(bool b);

	bool cursordLocked;

	Context* _currentContext;
};

