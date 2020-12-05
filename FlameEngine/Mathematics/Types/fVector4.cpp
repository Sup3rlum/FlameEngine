#include "FVector4.h"


float FVector4::Length() 
{
	return sqrt(LengthSquared());
}

constexpr float FVector4::LengthSquared()
{
	return x * x + y * y + z * z + w * w;
}

FVector4 FVector4::Normalize(FVector4 v)
{
	return v / v.Length();
}

STRING FVector4::ToString()
{
	return "X: " + std::to_string(x) + " Y: " + std::to_string(y) + " Z: " + std::to_string(z) + " W: " + std::to_string(w);
}


float const& FVector4::Dot(FVector4 const& l, FVector4 const& r)
{
	return l.x * r.x + l.y * r.y + l.z * r.z + l.w * r.w;
}


constexpr bool					operator==(FVector4 const& l, FVector4 const& r)
{
	return (
		l.x == r.x &&
		l.y == r.y &&
		l.z == r.z &&
		l.w == r.w
		);
}

constexpr bool					operator!=(FVector4 const& l, FVector4 const& r)
{
	return !(l == r);
}


constexpr FVector4			operator*(FVector4 const& l, FVector4 const& r)
{
	return FVector4(
		l.x * r.x,
		l.y * r.y,
		l.z * r.z,
		l.w * r.w
	);
}



constexpr FVector4			operator/(FVector4 const& l, FVector4 const& r)
{
	return FVector4(
		l.x / r.x,
		l.y / r.y,
		l.z / r.z,
		l.w / r.w
	);
}


constexpr FVector4			operator*(FVector4 const& l, float const& _scalar)
{
	return FVector4(
		l.x * _scalar,
		l.y * _scalar,
		l.z * _scalar,
		l.w * _scalar
	);
}


constexpr FVector4			operator/(FVector4 const& l, float const& _scalar)
{
	return FVector4(
		l.x / _scalar,
		l.y / _scalar,
		l.z / _scalar,
		l.w / _scalar
	);
}



constexpr FVector4			operator+(FVector4 const& l, FVector4 const& r)
{
	return FVector4(
		l.x + r.x,
		l.y + r.y,
		l.z + r.z,
		l.w + r.w
	);
}



constexpr FVector4			operator-(FVector4 const& l, FVector4 const& r)
{
	return FVector4(
		l.x - r.x,
		l.y - r.y,
		l.z - r.z,
		l.w - r.w
	);
}

