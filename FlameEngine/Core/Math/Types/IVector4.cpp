#include "IVector4.h"


float IVector4::Length() const
{
	return sqrt(LengthSquared());
}

constexpr float IVector4::LengthSquared() const
{
	return x * x + y * y + z * z + w * w;
}

IVector4 IVector4::Normalize(const IVector4& v)
{
	return v / v.Length();
}

FString IVector4::ToString() const
{
	return FString::Format("{ X:%0 Y:%1 Z:%2 W:%3 }", x, y, z, w);
}


int IVector4::Dot(IVector4 const& l, IVector4 const& r)
{
	return l.x * r.x + l.y * r.y + l.z * r.z + l.w * r.w;
}


constexpr bool					operator==(IVector4 const& l, IVector4 const& r)
{
	return (
		l.x == r.x &&
		l.y == r.y &&
		l.z == r.z &&
		l.w == r.w
		);
}

constexpr bool					operator!=(IVector4 const& l, IVector4 const& r)
{
	return !(l == r);
}


constexpr IVector4			operator*(IVector4 const& l, IVector4 const& r)
{
	return IVector4(
		l.x * r.x,
		l.y * r.y,
		l.z * r.z,
		l.w * r.w
	);
}



constexpr IVector4			operator/(IVector4 const& l, IVector4 const& r)
{
	return IVector4(
		l.x / r.x,
		l.y / r.y,
		l.z / r.z,
		l.w / r.w
	);
}


constexpr IVector4			operator*(IVector4 const& l, float const& _scalar)
{
	return IVector4(
		l.x * _scalar,
		l.y * _scalar,
		l.z * _scalar,
		l.w * _scalar
	);
}


constexpr IVector4			operator/(IVector4 const& l, float const& _scalar)
{
	return IVector4(
		l.x / _scalar,
		l.y / _scalar,
		l.z / _scalar,
		l.w / _scalar
	);
}



constexpr IVector4			operator+(IVector4 const& l, IVector4 const& r)
{
	return IVector4(
		l.x + r.x,
		l.y + r.y,
		l.z + r.z,
		l.w + r.w
	);
}



constexpr IVector4			operator-(IVector4 const& l, IVector4 const& r)
{
	return IVector4(
		l.x - r.x,
		l.y - r.y,
		l.z - r.z,
		l.w - r.w
	);
}

