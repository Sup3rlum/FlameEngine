#pragma once


#include "../../dll/nchfx.h"



EXPORT(class, fVector2)
{
public:
	union
	{
		struct { float x, y; };
		struct { float r, g; };
		struct { float s, t; };
	};

	constexpr fVector2()						: x(0), y(0) {}
	constexpr fVector2(float _v)				: x(_v), y(_v) {}
	constexpr fVector2(float _x, float _y)		: x(_x), y(_y) {}

	STRING ToString();

	float Length();
	constexpr float LengthSquared();


	static fVector2 const& Lerp(fVector2 const& l, fVector2 const& r, float const& t);

	static float const& Dot(fVector2 const& l, fVector2 const& r);

	static fVector2 Normalize(fVector2 v);


	constexpr float& operator[](_UNS_ FL_INT64 _index)
	{
		assert(_index < 2);

		if (_index == 0)
			return x;
		else
			return y;
	}

	constexpr fVector2& operator=(fVector2 const& v)
	{
		this->x = v.x;
		this->y = v.y;

		return *this;
	}


};

FLAME_DLL constexpr bool					operator==(fVector2 const& l, fVector2 const& r);
FLAME_DLL constexpr bool					operator!=(fVector2 const& l, fVector2 const& r);

FLAME_DLL constexpr fVector2			operator*(fVector2 const& l, fVector2 const& r);
FLAME_DLL constexpr fVector2			operator/(fVector2 const& l, fVector2 const& r);
FLAME_DLL constexpr fVector2			operator*(fVector2 const& l, float const& _scalar);
FLAME_DLL constexpr fVector2			operator/(fVector2 const& l, float const& _scalar);
FLAME_DLL constexpr fVector2			operator+(fVector2 const& l, fVector2 const& r);
FLAME_DLL constexpr fVector2			operator-(fVector2 const& l, fVector2 const& r);