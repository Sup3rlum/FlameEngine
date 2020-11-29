#pragma once


#include "../../dll/nchfx.h"
#include "fVector2.h"


EXPORT(class, fVector3) 
{
public:
	
	union
	{
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float s, t, p; };
		struct { fVector2 xy; float z; };
		struct { fVector2 rg; float b; };
		struct { fVector2 st; float p; };
		struct { float x;  fVector2 yz; };
		struct { float r;  fVector2 gb; };
		struct { float s;  fVector2 tp; };
	};


	constexpr fVector3() : x(0), y(0), z(0) {}

	constexpr fVector3(float _v) : x(_v), y(_v), z(_v) {}
	constexpr fVector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	constexpr fVector3(fVector2 _vec, float _z) : x(_vec.x), y(_vec.y), z(_z) {}



	STRING ToString();

	float Length();
	constexpr float LengthSquared();

	static fVector3 Lerp(fVector3 const& l, fVector3 const& r, float const& t);

	static fVector3 Cross(fVector3 const& l, fVector3 const& r);
	static float const& Dot(fVector3 const& l, fVector3 const& r);

	static fVector3 Normalize(fVector3 v);

	constexpr float& operator[](_UNS_ FL_INT64 _index)
	{
		assert(_index < 3);

		if (_index == 0)
			return x;
		else if (_index == 1)
			return y;
		else
			return z;
	}

	constexpr fVector3& operator=(fVector3 const& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;

		return *this;
	}

	constexpr fVector3& operator+=(fVector3 const& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;

		return *this;
	}
	constexpr fVector3& operator-=(fVector3 const& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;

		return *this;
	}

	constexpr fVector3& operator*=(float const& s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;

		return *this;
	}


};


FLAME_DLL fVector3			operator-(fVector3 const& l);

FLAME_DLL bool					operator==(fVector3 const& l, fVector3 const& r);
FLAME_DLL bool					operator!=(fVector3 const& l, fVector3 const& r);

FLAME_DLL fVector3			operator*(fVector3 const& l, fVector3 const& r);
FLAME_DLL fVector3			operator/(fVector3 const& l, fVector3 const& r);
FLAME_DLL fVector3			operator*(fVector3 const& l, float const& _scalar);
FLAME_DLL fVector3			operator/(fVector3 const& l, float const& _scalar);
FLAME_DLL fVector3			operator+(fVector3 const& l, fVector3 const& r);
FLAME_DLL fVector3			operator-(fVector3 const& l, fVector3 const& r);
FLAME_DLL fVector3			operator^(fVector3 const& l, fVector3 const& r);
FLAME_DLL float			operator&(fVector3 const& l, fVector3 const& r);