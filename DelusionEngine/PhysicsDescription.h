#pragma once

#include "nchfx.h"


EXPORT_ENUM PhysicsType
{
	STATIC = 0,
	BAKED = 1,
	DYNAMIC = 2
};

EXPORT_STRUCT PhysicsDescription
{
public:
	PhysicsType _physicsType;

	Vector3 _position;
	Vector3 _velocity;
	Quaternion _rotation;
};

