#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
	Min = Max = vec3(0);
}

BoundingBox::BoundingBox(vec3 min, vec3 max)
{
	Min = min;
	Max = max;
	Rotation = identity<mat4>();
}

BoundingBox::BoundingBox(vec3 min, vec3 max, mat4 rot)
{
	Min = min;
	Max = max;
	Rotation = rot;
}

vec3* BoundingBox::GetCorners()
{
	vec3 _corns[8];

	vec3 _off = Max - Min;

	_corns[0] = Min;
	_corns[1] = Min + vec3(_off.x, 0, 0);
	_corns[2] = Min + vec3(0, _off.y, 0);
	_corns[3] = Min + vec3(0, 0, _off.z);

	_corns[4] = Min + vec3(_off.x, 0, _off.z);
	_corns[5] = Min + vec3(_off.x, _off.y, 0);
	_corns[6] = Min + vec3(0, _off.y, _off.z);
	_corns[7] = Max;

	return _corns;
}

bool BoundingBox::Contains(vec3 p)
{
	return	(p.x > Min.x) &&
		(p.y > Min.y) &&
		(p.z > Min.z) &&

		(p.x < Max.x) &&
		(p.y < Max.y) &&
		(p.z < Max.z);
}