#include "fVector4.h"


float fVector4::Length() 
{
	return sqrt(LengthSquared());
}

constexpr float fVector4::LengthSquared()
{
	return x * x + y * y + z * z + w * w;
}

fVector4 fVector4::Normalize(fVector4 v)
{
	return v / v.Length();
}

STRING fVector4::ToString()
{
	return "X: " + std::to_string(x) + " Y: " + std::to_string(y) + " Z: " + std::to_string(z) + "W: " + std::to_string(w);
}


float const& fVector4::Dot(fVector4 const& l, fVector4 const& r)
{
	return l.x * r.x + l.y * r.y + l.z * r.z + l.w * r.w;
}


constexpr bool					operator==(fVector4 const& l, fVector4 const& r)
{
	return (
		l.x == r.x &&
		l.y == r.y &&
		l.z == r.z &&
		l.w == r.w
		);
}

constexpr bool					operator!=(fVector4 const& l, fVector4 const& r)
{
	return !(l == r);
}


constexpr fVector4			operator*(fVector4 const& l, fVector4 const& r)
{
	return fVector4(
		l.x * r.x,
		l.y * r.y,
		l.z * r.z,
		l.w * r.w
	);
}



constexpr fVector4			operator/(fVector4 const& l, fVector4 const& r)
{
	return fVector4(
		l.x / r.x,
		l.y / r.y,
		l.z / r.z,
		l.w / r.w
	);
}


constexpr fVector4			operator*(fVector4 const& l, float const& _scalar)
{
	return fVector4(
		l.x * _scalar,
		l.y * _scalar,
		l.z * _scalar,
		l.w * _scalar
	);
}


constexpr fVector4			operator/(fVector4 const& l, float const& _scalar)
{
	return fVector4(
		l.x / _scalar,
		l.y / _scalar,
		l.z / _scalar,
		l.w / _scalar
	);
}



constexpr fVector4			operator+(fVector4 const& l, fVector4 const& r)
{
	return fVector4(
		l.x + r.x,
		l.y + r.y,
		l.z + r.z,
		l.w + r.w
	);
}



constexpr fVector4			operator-(fVector4 const& l, fVector4 const& r)
{
	return fVector4(
		l.x - r.x,
		l.y - r.y,
		l.z - r.z,
		l.w - r.w
	);
}

