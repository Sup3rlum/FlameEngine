#pragma once

#include "../dll/nchfx.h"

using namespace glm;

EXPORT_CLASS BoundingBox
{
public:
	vec3 Min;
	vec3 Max;
	mat4 Rotation;

	BoundingBox();
	BoundingBox(vec3 min, vec3 max);
	BoundingBox(vec3 min, vec3 max, mat4 rot);

	vec3* GetCorners();

	bool Contains(vec3 p);

};

