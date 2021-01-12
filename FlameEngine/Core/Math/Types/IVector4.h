#pragma once

#include "Core/Common/CoreCommon.h"
#include "../SIMD/simddef.h"


EXPORT(class, IVector4)
{
public:

	union
	{
		struct { int x, y, z, w; };
		struct { int r, g, b, a; };
		struct { int s, t, p, q; };
		/*
		struct { int x;  FVector2 yz; float w; };
		struct { int r;  FVector2 gb; float a; };
		struct { int s;  FVector2 tp; float q; };

		struct { FVector2 xy; FVector2 zw; };
		struct { FVector2 rg; FVector2 ba; };
		struct { FVector2 st; FVector2 pq; };

		struct { FVector3 xyz; float w; };
		struct { FVector3 rgb; float a; };
		struct { FVector3 stp; float q; };

		struct { float x; FVector3 yzw; };
		struct { float r; FVector3 gba; };
		struct { float s; FVector3 tpq; };*/

		struct { int data[4]; };
		struct { __m128 mmv; };

		;
	};

	constexpr IVector4() : x(0), y(0), z(0), w(0) {}

	IVector4(int _v) : x(_v), y(_v), z(_v), w(_v) {}

	IVector4(int _x, int _y, int _z, int _w) : x(_x), y(_y), z(_z), w(_w) {}


	IVector4(__m128 pv) : mmv(pv) {}



	FString ToString() const;


	float Length() const;
	float LengthSquared() const;


	static int Dot(const IVector4 & l, const IVector4 & r);
	static IVector4 Normalize(const IVector4 & v);



	int& operator[](size_t index)
	{
		assert(index < 4);

		return data[index];
	}

	const int& operator[](size_t index) const
	{
		assert(index < 4);

		return data[index];
	}


	IVector4& operator=(const IVector4 & v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		this->w = v.w;

		return *this;
	}


	IVector4& operator+=(const IVector4 & v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		this->w += v.w;

		return *this;
	}
	IVector4& operator-=(const IVector4 & v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		this->w -= v.w;

		return *this;
	}

	IVector4& operator*=(const float& s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;
		this->w *= s;

		return *this;
	}
	IVector4& operator/=(const float& s)
	{
		this->x /= s;
		this->y /= s;
		this->z /= s;
		this->w /= s;

		return *this;
	}
	operator __m128()
	{
		return mmv;
	}

};



FLAME_API bool					operator==(const IVector4& l, const IVector4& r);
FLAME_API bool					operator!=(const IVector4& l, const IVector4& r);

FLAME_API IVector4			operator*(const IVector4& l, const IVector4& r);
FLAME_API IVector4			operator/(const IVector4& l, const IVector4& r);
FLAME_API IVector4			operator*(const IVector4& l, const int& scalar);
FLAME_API IVector4			operator/(const IVector4& l, const int& scalar);
FLAME_API IVector4			operator+(const IVector4& l, const IVector4& r);
FLAME_API IVector4			operator-(const IVector4& l, const IVector4& r);