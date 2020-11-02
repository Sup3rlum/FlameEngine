#pragma once

#include "../dll/nchfx.h"

#include "../Engine/FrameTime.h"
#include "../Context/ContextDescription.h"
#include "../Context/Context.h"


using namespace glm;

EXPORT_ENUM CameraType
{
	STATIC = 1,
	FIRSTPERSON = 2,
	THIRDPERSON = 3,
	TRACKBALL = 4
};
EXPORT_ENUM ProjectionType
{
	PERSPECTIVE = 1,
	ORTHOGRAPHIC = 2
};


EXPORT_CLASS Camera
{

public:
	Camera(Context* _cont, CameraType _camType, ProjectionType _projType);
	~Camera();

	CameraType Type;
	ProjectionType ProjType;

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

	void Update();

	float mouseSpeed = 0.3f;
	float flySpeed = 100.0f;

	Context* _currentContext;
};

