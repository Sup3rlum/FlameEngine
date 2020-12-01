#include "fMatrix3.h"


fMatrix3 fMatrix3::Transpose(fMatrix3 m)
{
	fMatrix3 Result = m;

	Result[0][1] = m[1][0];
	Result[0][2] = m[2][0];

	Result[1][2] = m[2][1];
	Result[2][1] = m[1][2];

	Result[2][0] = m[0][2];
	Result[1][0] = m[0][1];

	return Result;
}

fVector3 operator*(fMatrix3 const& l, fVector3 const& r)
{
	fVector3 _v0 = l[0] * r;
	fVector3 _v1 = l[1] * r;
	fVector3 _v2 = l[2] * r;

	return fVector3
	(
		_v0.x + _v0.y + _v0.z,
		_v1.x + _v1.y + _v1.z,
		_v2.x + _v2.y + _v2.z
	);
}
