#pragma once


#include "../../dll/nchfx.h"



EXPORT(class, FVector2)
{
public:
	union
	{
		struct { float x, y; };
		struct { float r, g; };
		struct { float s, t; };
	};

	constexpr FVector2()						: x(0), y(0) {}
	constexpr FVector2(float _v)				: x(_v), y(_v) {}
	constexpr FVector2(float _x, float _y)		: x(_x), y(_y) {}

	STRING ToString();

	float Length();
	constexpr float LengthSquared();


	static FVector2 const& Lerp(FVector2 const& l, FVector2 const& r, float const& t);

	static float const& Dot(FVector2 const& l, FVector2 const& r);

	static FVector2 Normalize(FVector2 v);


	constexpr float& operator[](_UNS_ FL_INT64 _index)
	{
		assert(_index < 2);

		if (_index == 0)
			return x;
		else
			return y;
	}

	constexpr FVector2& operator=(FVector2 const& v)
	{
		this->x = v.x;
		this->y = v.y;

		return *this;
	}


};

FLAME_DLL constexpr bool					operator==(FVector2 const& l, FVector2 const& r);
FLAME_DLL constexpr bool					operator!=(FVector2 const& l, FVector2 const& r);

FLAME_DLL constexpr FVector2			operator*(FVector2 const& l, FVector2 const& r);
FLAME_DLL constexpr FVector2			operator/(FVector2 const& l, FVector2 const& r);
FLAME_DLL constexpr FVector2			operator*(FVector2 const& l, float const& _scalar);
FLAME_DLL constexpr FVector2			operator/(FVector2 const& l, float const& _scalar);
FLAME_DLL constexpr FVector2			operator+(FVector2 const& l, FVector2 const& r);
FLAME_DLL constexpr FVector2			operator-(FVector2 const& l, FVector2 const& r);