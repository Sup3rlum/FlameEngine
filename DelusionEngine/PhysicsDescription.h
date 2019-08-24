#pragma once

#include "nchfx.h"


enum class DELUSION_DLL PhysicsType
{
	STATIC = 0,
	BAKED = 1,
	DYNAMIC = 2
};

struct DELUSION_DLL PhysicsDescription
{
public:
	PhysicsType _physicsType;

	Vector3 _position;
	Vector3 _velocity;
	Quaternion _rotation;
};

