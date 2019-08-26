#pragma once

#include "nchfx.h"

#include "FrameTime.h"
#include "ContextDescription.h"
#include "BaseContext.h"


using namespace glm;

EXPORT_ENUM CameraType
{
	STATIC = 1,
	FIRSTPERSON = 2,
	THIRDPERSON = 3,
	TRACKBALL = 4
};


EXPORT_CLASS Camera
{

public:
	Camera(BaseContext* _cont, CameraType _camType);
	~Camera();

	CameraType Type;

	Matrix4 View;
	Matrix4 DebugView;
	Matrix4 Projection;
	Matrix4 ProjectionInverse;

	Vector3 Position;
	Vector3 Target;
	Vector3 Right;
	Vector3 Up;

	float Fov = 45.0f;

	float horizontalAngle;
	float verticalAngle;

	void Update(FrameTime* _frTime);

	float mouseSpeed = 0.3f;
	float flySpeed = 100.0f;

	BaseContext* _currentContext;
};

