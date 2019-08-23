#pragma once

#include "nchfx.h"

#include "FrameTime.h"
#include "ContextDescription.h"



using namespace glm;

class DELUSION_DLL Camera
{


public:
	Camera();
	~Camera();

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

	void Update(ContextDescription* _descr, FrameTime* _frTime);

	float mouseSpeed = 0.3f;
	float flySpeed = 100.0f;
};

