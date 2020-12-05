#pragma once


#include "../../dll/nchfx.h"
#include "FVector2.h"
#include "FVector3.h"


EXPORT(class, FVector4)
{
public:

	union
	{
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
		struct { float s, t, p, q; };

		struct { float x;  FVector2 yz; float w; };
		struct { float r;  FVector2 gb; float a; };
		struct { float s;  FVector2 tp; float q; };

		struct { FVector2 xy; FVector2 zw; };
		struct { FVector2 rg; FVector2 ba; };
		struct { FVector2 st; FVector2 pq; };

		struct { FVector3 xyz; float w; };
		struct { FVector3 rgb; float a; };
		struct { FVector3 stp; float q;  };

		struct { float x; FVector3 yzw; };
		struct { float r; FVector3 gba; };
		struct { float s; FVector3 tpq; };

		struct { __m128 mmv; };

;
	};

	constexpr FVector4() : x(0), y(0), z(0), w(0) {}

	constexpr FVector4(float _v) : x(_v), y(_v), z(_v), w(_v) {}

	constexpr FVector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

	constexpr FVector4(FVector3 _vec, float _w) : x(_vec.x), y(_vec.y), z(_vec.z), w(_w) {}

	constexpr FVector4(FVector2 _vec1, FVector2 _vec2) : x(_vec1.x), y(_vec1.y), z(_vec2.x), w(_vec2.y) {}

	constexpr FVector4(__m128 pv) : mmv(pv) {}



	STRING ToString();


	float Length(); 
	constexpr float LengthSquared();


	static float const& Dot(FVector4 const& l, FVector4 const& r);

	static FVector4 Normalize(FVector4 v);



	float& operator[](_UNS_ FL_INT64 _index)
	{
		assert(_index < 4);

		if (_index == 0)
			return x;
		else if (_index == 1)
			return y;
		else if (_index == 2)
			return z;
		else
			return w;
	}

	constexpr float const& operator[](_UNS_ FL_INT64 _index) const
	{
		assert(_index < 4);

		if (_index == 0)
			return x;
		else if (_index == 1)
			return y;
		else if (_index == 2)
			return z;
		else
			return w;
	}


	constexpr FVector4& operator=(FVector4 const& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		this->w = v.w;

		return *this;
	}


	constexpr FVector4& operator+=(FVector4 const& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		this->w += v.w;

		return *this;
	}
	constexpr FVector4& operator-=(FVector4 const& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		this->w -= v.w;

		return *this;
	}

	constexpr FVector4& operator*=(float const& s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;
		this->w *= s;

		return *this;
	}
	constexpr FVector4& operator/=(float const& s)
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



FLAME_DLL constexpr bool					operator==(FVector4 const& l, FVector4 const& r);
FLAME_DLL constexpr bool					operator!=(FVector4 const& l, FVector4 const& r);

FLAME_DLL constexpr FVector4			operator*(FVector4 const& l, FVector4 const& r);
FLAME_DLL constexpr FVector4			operator/(FVector4 const& l, FVector4 const& r);
FLAME_DLL constexpr FVector4			operator*(FVector4 const& l, float const& _scalar);
FLAME_DLL constexpr FVector4			operator/(FVector4 const& l, float const& _scalar);
FLAME_DLL constexpr FVector4			operator+(FVector4 const& l, FVector4 const& r);
FLAME_DLL constexpr FVector4			operator-(FVector4 const& l, FVector4 const& r);