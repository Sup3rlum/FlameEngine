#pragma once

#include "../dll/nchfx.h"

#include "Plane.h"


EXPORT_CLASS Ray
{
public:
	Vector3 Position;
	Vector3 Direction;

	Ray(Vector3 p, Vector3 d);


	bool Intersects(Plane pl, Vector3 &out);
};