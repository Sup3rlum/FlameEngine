#include "fQuaternion.h"



float fQuaternion::Length()
{
	return sqrt(LengthSquared());
}

constexpr float fQuaternion::LengthSquared()
{
	return x * x + y * y + z * z + w * w;
}


fQuaternion fQuaternion::Normalize(fQuaternion v)
{
	return v / v.Length();
}

fQuaternion fQuaternion::Identity()
{
	return fQuaternion(1, 0, 0, 0);
}

fQuaternion const& fQuaternion::Inverse(fQuaternion q)
{
	return fQuaternion(q.r, -q.i, -q.j, -q.j) / q.Length();
}

constexpr bool					operator==(fQuaternion const& l, fQuaternion const& r)
{
	return (
		l.r == r.r &&
		l.i == r.i &&
		l.j == r.j &&
		l.k == r.k
		);
}

constexpr bool					operator!=(fQuaternion const& l, fQuaternion const& r)
{
	return !(l == r);
}


constexpr fQuaternion			operator*(fQuaternion const& l, fQuaternion const& r)
{
	return fQuaternion(
		(l.r * r.r) - (l.ijk & r.ijk),
		l.ijk ^ r.ijk + r.ijk * l.r + l.ijk * r.r
	);
}



constexpr fQuaternion			operator*(fQuaternion const& l, float const& _scalar)
{
	return fQuaternion(
		l.r * _scalar,
		l.i * _scalar,
		l.j * _scalar,
		l.k * _scalar
	);
}


constexpr fQuaternion			operator/(fQuaternion const& l, float const& _scalar)
{
	return fQuaternion(
		l.r / _scalar,
		l.i / _scalar,
		l.j / _scalar,
		l.k / _scalar
	);
}



constexpr fQuaternion			operator+(fQuaternion const& l, fQuaternion const& r)
{
	return fQuaternion(
		l.r + r.r,
		l.i + r.i,
		l.j + r.j,
		l.k + r.k
	);
}



constexpr fQuaternion			operator-(fQuaternion const& l, fQuaternion const& r)
{
	return fQuaternion(
		l.r - r.r,
		l.i - r.i,
		l.j - r.j,
		l.k - r.k
	);
}

