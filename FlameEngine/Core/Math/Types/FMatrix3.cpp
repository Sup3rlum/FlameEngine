#include "FMatrix3.h"


FMatrix3 FMatrix3::Transpose(FMatrix3 m)
{
	FMatrix3 Result = m;

	Result[0][1] = m[1][0];
	Result[0][2] = m[2][0];

	Result[1][2] = m[2][1];
	Result[2][1] = m[1][2];

	Result[2][0] = m[0][2];
	Result[1][0] = m[0][1];

	return Result;
}

FVector3 operator*(FMatrix3 const& l, FVector3 const& r)
{
	FVector3 _v0 = l[0] * r;
	FVector3 _v1 = l[1] * r;
	FVector3 _v2 = l[2] * r;

	return FVector3
	(
		_v0.x + _v0.y + _v0.z,
		_v1.x + _v1.y + _v1.z,
		_v2.x + _v2.y + _v2.z
	);
}
