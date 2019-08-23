#pragma once

#include "nchfx.h"

#include "FrameTime.h"
#include "ContextParameters.h"



using namespace glm;

class DELUSION_DLL Camera
{


public:
	Camera();
	~Camera();

	mat4x4 View;
	mat4x4 DebugView;
	mat4x4 Projection;
	mat4x4 ProjectionInverse;
	vec3 Position;
	vec3 Target;
	vec3 Right;
	vec3 Up;

	float Fov = 45.0f;

	float horizontalAngle;
	float verticalAngle;

	void Update(GLFWwindow* _window, ContextParameters* _params, FrameTime* _frTime);

	float mouseSpeed = 0.3f;
	float flySpeed = 100.0f;
};

