#include "FVector3.h"


float FVector3::Length()
{
	return sqrt(LengthSquared());
}

constexpr float FVector3::LengthSquared()
{
	return x * x + y * y + z * z;
}

FString FVector3::ToString()
{
	return "X: " + std::to_string(x) + " Y: " + std::to_string(y) + " Z: " + std::to_string(z);
}


FVector3 FVector3::Normalize(FVector3  v)
{
	return v / v.Length();
}

FVector3 FVector3::Lerp(FVector3 const& l, FVector3 const& r, float const& t)
{
	return l + (r - l) * t;
}

FVector3 FVector3::Cross(FVector3 const& l, FVector3 const& r)
{
	return FVector3(
		l.y * r.z - l.z * r.y,
		l.z * r.x - l.x * r.z,
		l.x * r.y - l.y * r.x
	);
}
float const& FVector3::Dot(FVector3 const& l, FVector3 const& r)
{
	return l.x * r.x + l.y * r.y + l.z * r.z;
}


FVector3 FVector3::Max(FVector3 a, FVector3 b)
{
	return FVector3(
		max(a.x, b.x),
		max(a.y, b.y),
		max(a.z, b.z)
		);
}

FVector3 FVector3::Min(FVector3 a, FVector3 b)
{
	return FVector3(
		min(a.x, b.x),
		min(a.y, b.y),
		min(a.z, b.z)
	);
}


FVector3			operator-(FVector3 const& l)
{
	return FVector3(
		-l.x,
		-l.y,
		-l.z
		);
}


bool					operator==(FVector3 const& l, FVector3 const& r)
{
	return (
		l.x == r.x &&
		l.y == r.y &&
		l.z == r.z
		);
}

bool					operator!=(FVector3 const& l, FVector3 const& r)
{
	return !(l == r);
}


FVector3			operator*(FVector3 const& l, FVector3 const& r)
{
	return FVector3(
		l.x * r.x,
		l.y * r.y,
		l.z * r.z
	);
}



FVector3			operator/(FVector3 const& l, FVector3 const& r)
{
	return FVector3(
		l.x / r.x,
		l.y / r.y,
		l.z / r.z
	);
}


FVector3			operator*(FVector3 const& l, float const& _scalar)
{
	return FVector3(
		l.x * _scalar,
		l.y * _scalar,
		l.z * _scalar
	);
}


FVector3			operator/(FVector3 const& l, float const& _scalar)
{
	return FVector3(
		l.x / _scalar,
		l.y / _scalar,
		l.z / _scalar
	);
}



FVector3			operator+(FVector3 const& l, FVector3 const& r)
{
	return FVector3(
		l.x + r.x,
		l.y + r.y,
		l.z + r.z
	);
}



FVector3			operator-(FVector3 const& l, FVector3 const& r)
{
	return FVector3(
		l.x - r.x,
		l.y - r.y,
		l.z - r.z
	);
}

FVector3			operator^(FVector3 const& l, FVector3 const& r)
{
	return FVector3::Cross(l, r);
}

float			operator&(FVector3 const& l, FVector3 const& r)
{
	return FVector3::Dot(l, r);
}

