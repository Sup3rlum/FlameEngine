#pragma once


#include "../../dll/nchfx.h"
#include "fVector2.h"
#include "fVector3.h"


EXPORT(class, fVector4)
{
public:

	union
	{
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
		struct { float s, t, p, q; };

		struct { float x;  fVector2 yz; float w; };
		struct { float r;  fVector2 gb; float a; };
		struct { float s;  fVector2 tp; float q; };

		struct { fVector2 xy; fVector2 zw; };
		struct { fVector2 rg; fVector2 ba; };
		struct { fVector2 st; fVector2 pq; };

		struct { fVector3 xyz; float w; };
		struct { fVector3 rgb; float a; };
		struct { fVector3 stp; float q;  };

		struct { float x; fVector3 yzw; };
		struct { float r; fVector3 gba; };
		struct { float s; fVector3 tpq; };

		struct { __m128 mmv; };

;
	};

	constexpr fVector4() : x(0), y(0), z(0), w(0) {}

	constexpr fVector4(float _v) : x(_v), y(_v), z(_v), w(_v) {}

	constexpr fVector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

	constexpr fVector4(fVector3 _vec, float _w) : x(_vec.x), y(_vec.y), z(_vec.z), w(_w) {}

	constexpr fVector4(fVector2 _vec1, fVector2 _vec2) : x(_vec1.x), y(_vec1.y), z(_vec2.x), w(_vec2.y) {}

	constexpr fVector4(__m128 pv) : mmv(pv) {}






	float Length(); 
	constexpr float LengthSquared();


	static float const& Dot(fVector4 const& l, fVector4 const& r);

	static fVector4 Normalize(fVector4 v);



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


	constexpr fVector4& operator=(fVector4 const& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		this->w = v.w;

		return *this;
	}


	constexpr fVector4& operator+=(fVector4 const& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		this->w += v.w;

		return *this;
	}
	constexpr fVector4& operator-=(fVector4 const& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		this->w -= v.w;

		return *this;
	}

	constexpr fVector4& operator*=(float const& s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;
		this->w *= s;

		return *this;
	}

	operator __m128()
	{
		return mmv;
	}

};



FLAME_DLL constexpr bool					operator==(fVector4 const& l, fVector4 const& r);
FLAME_DLL constexpr bool					operator!=(fVector4 const& l, fVector4 const& r);

FLAME_DLL constexpr fVector4			operator*(fVector4 const& l, fVector4 const& r);
FLAME_DLL constexpr fVector4			operator/(fVector4 const& l, fVector4 const& r);
FLAME_DLL constexpr fVector4			operator*(fVector4 const& l, float const& _scalar);
FLAME_DLL constexpr fVector4			operator/(fVector4 const& l, float const& _scalar);
FLAME_DLL constexpr fVector4			operator+(fVector4 const& l, fVector4 const& r);
FLAME_DLL constexpr fVector4			operator-(fVector4 const& l, fVector4 const& r);