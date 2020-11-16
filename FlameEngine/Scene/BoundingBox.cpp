#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
	Min = Max = fVector3(0);
}

BoundingBox::BoundingBox(fVector3 min, fVector3 max)
{
	Min = min;
	Max = max;
	Rotation = fMatrix4::Identity();
}

BoundingBox::BoundingBox(fVector3 min, fVector3 max, fMatrix4 rot)
{
	Min = min;
	Max = max;
	Rotation = rot;
}

fVector3* BoundingBox::GetCorners()
{
	fVector3 _corns[8];

	fVector3 _off = Max - Min;

	_corns[0] = Min;
	_corns[1] = Min + fVector3(_off.x, 0, 0);
	_corns[2] = Min + fVector3(0, _off.y, 0);
	_corns[3] = Min + fVector3(0, 0, _off.z);

	_corns[4] = Min + fVector3(_off.x, 0, _off.z);
	_corns[5] = Min + fVector3(_off.x, _off.y, 0);
	_corns[6] = Min + fVector3(0, _off.y, _off.z);
	_corns[7] = Max;

	return _corns;
}

bool BoundingBox::Contains(fVector3 p)
{
	return	(p.x > Min.x) &&
		(p.y > Min.y) &&
		(p.z > Min.z) &&

		(p.x < Max.x) &&
		(p.y < Max.y) &&
		(p.z < Max.z);
}