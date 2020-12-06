#include "FMatrix2.h"


FMatrix2 FMatrix2::Transpose(FMatrix2 m)
{
	FMatrix2 Result = m;

	Result[0][1] = m[1][0];
	Result[1][0] = m[0][1];

	return Result;
}
FMatrix2 FMatrix2::Inverse(FMatrix2 m)
{
	FMatrix2 Result = m;

	Result[0][0] = m[1][1];
	Result[1][1] = m[0][0];

	Result[0][1] = -m[0][1];
	Result[1][0] = -m[1][0];

	float invdet = 1.0f / Determinant(m);

	return invdet * Result;
}

float FMatrix2::Determinant(FMatrix2 m)
{
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}


FVector2 operator*(FMatrix2 const& l, FVector2 const& r)
{
	FVector2 _v0 = l[0] * r;
	FVector2 _v1 = l[1] * r;

	return FVector2
	(
		_v0.x + _v0.y,
		_v1.x + _v1.y
	);
}

FMatrix2 operator*(float const& l, FMatrix2 const& r)
{
	return FMatrix2
	(
		r[0] * l,
		r[1] * l
	);
}
