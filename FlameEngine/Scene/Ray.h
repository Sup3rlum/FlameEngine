#pragma once

#include "../dll/nchfx.h"

#include "Plane.h"


EXPORT_CLASS Ray
{
public:
	fVector3 Position;
	fVector3 Direction;

	Ray(fVector3 p, fVector3 d);


	bool Intersects(Plane pl, fVector3 &out);
};