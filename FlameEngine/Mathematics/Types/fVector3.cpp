#include "fVector3.h"


float fVector3::Length()
{
	return sqrt(LengthSquared());
}

constexpr float fVector3::LengthSquared()
{
	return x * x + y * y + z * z;
}




fVector3 fVector3::Normalize(fVector3  v)
{
	return v / v.Length();
}

fVector3 const& fVector3::Lerp(fVector3 const& l, fVector3 const& r, float const& t)
{
	return l + (r - l) * t;
}

fVector3 const& fVector3::Cross(fVector3 const& l, fVector3 const& r)
{
	return fVector3(
		l.y * r.z - l.z * r.y,
		l.z * r.x - l.x * r.z,
		l.x * r.y - l.y * r.x
	);
}
float const& fVector3::Dot(fVector3 const& l, fVector3 const& r)
{
	return l.x * r.x + l.y * r.y + l.z * r.z;
}


fVector3			operator-(fVector3 const& l)
{
	return (
		-l.x,
		-l.y,
		-l.z
		);
}


bool					operator==(fVector3 const& l, fVector3 const& r)
{
	return (
		l.x == r.x &&
		l.y == r.y &&
		l.z == r.z
		);
}

bool					operator!=(fVector3 const& l, fVector3 const& r)
{
	return !(l == r);
}


fVector3			operator*(fVector3 const& l, fVector3 const& r)
{
	return fVector3(
		l.x * r.x,
		l.y * r.y,
		l.z * r.z
	);
}



fVector3			operator/(fVector3 const& l, fVector3 const& r)
{
	return fVector3(
		l.x / r.x,
		l.y / r.y,
		l.z / r.z
	);
}


fVector3			operator*(fVector3 const& l, float const& _scalar)
{
	return fVector3(
		l.x * _scalar,
		l.y * _scalar,
		l.z * _scalar
	);
}


fVector3			operator/(fVector3 const& l, float const& _scalar)
{
	return fVector3(
		l.x / _scalar,
		l.y / _scalar,
		l.z / _scalar
	);
}



fVector3			operator+(fVector3 const& l, fVector3 const& r)
{
	return fVector3(
		l.x + r.x,
		l.y + r.y,
		l.z + r.z
	);
}



fVector3			operator-(fVector3 const& l, fVector3 const& r)
{
	return fVector3(
		l.x - r.x,
		l.y - r.y,
		l.z - r.z
	);
}

fVector3			operator^(fVector3 const& l, fVector3 const& r)
{
	return fVector3::Cross(l, r);
}

float			operator&(fVector3 const& l, fVector3 const& r)
{
	return fVector3::Dot(l, r);
}

