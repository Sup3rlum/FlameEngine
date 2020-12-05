#include "fVector2.h"





float fVector2::Length()
{
	return sqrt(LengthSquared());
}

constexpr float fVector2::LengthSquared() 
{
	return x * x + y * y;
}

fVector2 fVector2::Normalize(fVector2  v)
{
	return v / v.Length();
}

fVector2 const& fVector2::Lerp(fVector2 const& l, fVector2 const& r, float const& t)
{
	return l + (r - l) * t;
}


STRING fVector2::ToString()
{
	return "X: " + std::to_string(x) + " Y: " + std::to_string(y);
}




float const& fVector2::Dot(fVector2 const& l, fVector2 const& r)
{
	return l.x * r.x + l.y * r.y;
}


constexpr bool					operator==(fVector2 const& l, fVector2 const& r)
{
	return (l.x == r.x && l.y == r.y);
}

constexpr bool					operator!=(fVector2 const& l, fVector2 const& r)
{
	return !(l == r);
}



constexpr fVector2			operator*(fVector2 const& l, fVector2 const& r)
{
	return fVector2(l.x * r.x, l.y * r.y);
}



constexpr fVector2			operator/(fVector2 const& l, fVector2 const& r)
{
	return fVector2(l.x / r.x, l.y / r.y);
}


constexpr fVector2			operator*(fVector2 const& l, float const& _scalar)
{
	return fVector2(l.x * _scalar, l.y * _scalar);
}


constexpr fVector2			operator/(fVector2 const& l, float const& _scalar)
{
	return fVector2(l.x / _scalar, l.y / _scalar);
}



constexpr fVector2			operator+(fVector2 const& l, fVector2 const& r)
{
	return fVector2(l.x + r.x, l.y + r.y);
}



constexpr fVector2			operator-(fVector2 const& l, fVector2 const& r)
{
	return fVector2(l.x - r.x, l.y - r.y);
}

