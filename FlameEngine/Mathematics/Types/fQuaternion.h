#pragma once

#include "../../dll/nchfx.h"
#include "fVector2.h"
#include "fVector3.h"
#include "fVector4.h"

EXPORT(class,  fQuaternion)
{
public:
	union
	{
		struct { float w, x, y, z; };
		struct { float r, i, j, k; };


		struct { float w;  fVector2 xy; float z; };
		struct { float r;  fVector2 ij; float k; };


		struct { fVector2 wx; fVector2 yz; };
		struct { fVector2 ri; fVector2 jk; };


		struct { fVector3 wxy; float z; };
		struct { fVector3 rij; float k; };


		struct { float w; fVector3 xyz; };
		struct { float r; fVector3 ijk; };

		struct { fVector4 vector4; };
		struct { __m128 mmv; };
	};
	
	constexpr fQuaternion() :
		i(0),
		j(0),
		k(0),
		r(0) {}
	constexpr fQuaternion(float _r, float _i, float _j, float _k) :
		i(_i),
		j(_j),
		k(_k),
		r(_r) {}

	constexpr fQuaternion(float _r, fVector3 _v) :
		i(_v.x),
		j(_v.y),
		k(_v.z),
		r(_r) {}



	constexpr fQuaternion(fVector4 _vec) :
		vector4(_vec) {}




	float Length();
	constexpr float LengthSquared();



	static fQuaternion Normalize(fQuaternion q);
	static fQuaternion Identity();
	static fQuaternion const& Inverse(fQuaternion q);





	float& operator[](_UNS_ FL_INT64 _index)
	{
		assert(_index < 4);

		if (_index == 0)
			return r;
		else if (_index == 1)
			return i;
		else if (_index == 2)
			return j;
		else
			return k;
	}

	constexpr float const& operator[](_UNS_ FL_INT64 _index) const
	{
		assert(_index < 4);

		if (_index == 0)
			return r;
		else if (_index == 1)
			return i;
		else if (_index == 2)
			return j;
		else
			return k;
	}


	constexpr fQuaternion& operator=(fQuaternion const& v)
	{
		this->r = v.r;
		this->i = v.i;
		this->j = v.j;
		this->k = v.k;

		return *this;
	}


};


constexpr bool		FLAME_DLL			operator==(fQuaternion const& l, fQuaternion const& r);
constexpr bool		FLAME_DLL			operator!=(fQuaternion const& l, fQuaternion const& r);

constexpr fQuaternion	FLAME_DLL		operator*(fQuaternion const& l, fQuaternion const& r);
constexpr fQuaternion	FLAME_DLL		operator*(fQuaternion const& l, float const& _scalar);
constexpr fQuaternion	FLAME_DLL		operator/(fQuaternion const& l, float const& _scalar);
constexpr fQuaternion	FLAME_DLL		operator+(fQuaternion const& l, fQuaternion const& r);
constexpr fQuaternion	FLAME_DLL		operator-(fQuaternion const& l, fQuaternion const& r);