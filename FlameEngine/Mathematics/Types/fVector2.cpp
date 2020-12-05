#include "FVector2.h"





float FVector2::Length()
{
	return sqrt(LengthSquared());
}

constexpr float FVector2::LengthSquared() 
{
	return x * x + y * y;
}

FVector2 FVector2::Normalize(FVector2  v)
{
	return v / v.Length();
}

FVector2 const& FVector2::Lerp(FVector2 const& l, FVector2 const& r, float const& t)
{
	return l + (r - l) * t;
}


STRING FVector2::ToString()
{
	return "X: " + std::to_string(x) + " Y: " + std::to_string(y);
}




float const& FVector2::Dot(FVector2 const& l, FVector2 const& r)
{
	return l.x * r.x + l.y * r.y;
}


constexpr bool					operator==(FVector2 const& l, FVector2 const& r)
{
	return (l.x == r.x && l.y == r.y);
}

constexpr bool					operator!=(FVector2 const& l, FVector2 const& r)
{
	return !(l == r);
}



constexpr FVector2			operator*(FVector2 const& l, FVector2 const& r)
{
	return FVector2(l.x * r.x, l.y * r.y);
}



constexpr FVector2			operator/(FVector2 const& l, FVector2 const& r)
{
	return FVector2(l.x / r.x, l.y / r.y);
}


constexpr FVector2			operator*(FVector2 const& l, float const& _scalar)
{
	return FVector2(l.x * _scalar, l.y * _scalar);
}


constexpr FVector2			operator/(FVector2 const& l, float const& _scalar)
{
	return FVector2(l.x / _scalar, l.y / _scalar);
}



constexpr FVector2			operator+(FVector2 const& l, FVector2 const& r)
{
	return FVector2(l.x + r.x, l.y + r.y);
}



constexpr FVector2			operator-(FVector2 const& l, FVector2 const& r)
{
	return FVector2(l.x - r.x, l.y - r.y);
}

