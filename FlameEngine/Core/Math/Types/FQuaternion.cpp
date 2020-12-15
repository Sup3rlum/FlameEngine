#include "FQuaternion.h"



float FQuaternion::Length()
{
	return sqrt(LengthSquared());
}

constexpr float FQuaternion::LengthSquared()
{
	return x * x + y * y + z * z + w * w;
}


FQuaternion FQuaternion::Normalize(FQuaternion v)
{
	return v / v.Length();
}

FQuaternion FQuaternion::Identity()
{
	return FQuaternion(1, 0, 0, 0);
}

FQuaternion const& FQuaternion::Inverse(FQuaternion q)
{
	return FQuaternion(q.r, -q.i, -q.j, -q.j) / q.Length();
}

constexpr bool					operator==(FQuaternion const& l, FQuaternion const& r)
{
	return (
		l.r == r.r &&
		l.i == r.i &&
		l.j == r.j &&
		l.k == r.k
		);
}

constexpr bool					operator!=(FQuaternion const& l, FQuaternion const& r)
{
	return !(l == r);
}


constexpr FQuaternion			operator*(FQuaternion const& l, FQuaternion const& r)
{
	return FQuaternion(
		(l.r * r.r) - (l.ijk & r.ijk),
		l.ijk ^ r.ijk + r.ijk * l.r + l.ijk * r.r
	);
}



constexpr FQuaternion			operator*(FQuaternion const& l, float const& _scalar)
{
	return FQuaternion(
		l.r * _scalar,
		l.i * _scalar,
		l.j * _scalar,
		l.k * _scalar
	);
}


constexpr FQuaternion			operator/(FQuaternion const& l, float const& _scalar)
{
	return FQuaternion(
		l.r / _scalar,
		l.i / _scalar,
		l.j / _scalar,
		l.k / _scalar
	);
}



constexpr FQuaternion			operator+(FQuaternion const& l, FQuaternion const& r)
{
	return FQuaternion(
		l.r + r.r,
		l.i + r.i,
		l.j + r.j,
		l.k + r.k
	);
}



constexpr FQuaternion			operator-(FQuaternion const& l, FQuaternion const& r)
{
	return FQuaternion(
		l.r - r.r,
		l.i - r.i,
		l.j - r.j,
		l.k - r.k
	);
}

