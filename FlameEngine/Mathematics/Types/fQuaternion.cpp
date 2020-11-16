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

fQuaternion const& fQuaternion::Identity()
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
		l.x == r.x &&
		l.y == r.y &&
		l.z == r.z &&
		l.w == r.w
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
		l.x * _scalar,
		l.y * _scalar,
		l.z * _scalar,
		l.w * _scalar
	);
}


constexpr fQuaternion			operator/(fQuaternion const& l, float const& _scalar)
{
	return fQuaternion(
		l.x / _scalar,
		l.y / _scalar,
		l.z / _scalar,
		l.w / _scalar
	);
}



constexpr fQuaternion			operator+(fQuaternion const& l, fQuaternion const& r)
{
	return fQuaternion(
		l.x + r.x,
		l.y + r.y,
		l.z + r.z,
		l.w + r.w
	);
}



constexpr fQuaternion			operator-(fQuaternion const& l, fQuaternion const& r)
{
	return fQuaternion(
		l.x - r.x,
		l.y - r.y,
		l.z - r.z,
		l.w - r.w
	);
}

