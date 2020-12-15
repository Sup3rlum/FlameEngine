#pragma once

#include "Core/Common/CoreCommon.h"
#include "FVector2.h"
#include "FVector3.h"
#include "FVector4.h"

EXPORT(class,  FQuaternion)
{
public:
	union
	{
		struct { float w, x, y, z; };
		struct { float r, i, j, k; };


		struct { float w;  FVector2 xy; float z; };
		struct { float r;  FVector2 ij; float k; };


		struct { FVector2 wx; FVector2 yz; };
		struct { FVector2 ri; FVector2 jk; };


		struct { FVector3 wxy; float z; };
		struct { FVector3 rij; float k; };


		struct { float w; FVector3 xyz; };
		struct { float r; FVector3 ijk; };

		struct { FVector4 vector4; };
		struct { __m128 mmv; };
	};
	
	constexpr FQuaternion() :
		i(0),
		j(0),
		k(0),
		r(0) {}
	constexpr FQuaternion(float _r, float _i, float _j, float _k) :
		i(_i),
		j(_j),
		k(_k),
		r(_r) {}

	constexpr FQuaternion(float _r, FVector3 _v) :
		i(_v.x),
		j(_v.y),
		k(_v.z),
		r(_r) {}



	constexpr FQuaternion(FVector4 _vec) :
		vector4(_vec) {}




	float Length();
	constexpr float LengthSquared();



	static FQuaternion Normalize(FQuaternion q);
	static FQuaternion Identity();
	static FQuaternion const& Inverse(FQuaternion q);





	float& operator[](size_t _index)
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

	constexpr float const& operator[](size_t _index) const
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


	constexpr FQuaternion& operator=(FQuaternion const& v)
	{
		this->r = v.r;
		this->i = v.i;
		this->j = v.j;
		this->k = v.k;

		return *this;
	}


};


constexpr bool		FLAME_API			operator==(FQuaternion const& l, FQuaternion const& r);
constexpr bool		FLAME_API			operator!=(FQuaternion const& l, FQuaternion const& r);

constexpr FQuaternion	FLAME_API		operator*(FQuaternion const& l, FQuaternion const& r);
constexpr FQuaternion	FLAME_API		operator*(FQuaternion const& l, float const& _scalar);
constexpr FQuaternion	FLAME_API		operator/(FQuaternion const& l, float const& _scalar);
constexpr FQuaternion	FLAME_API		operator+(FQuaternion const& l, FQuaternion const& r);
constexpr FQuaternion	FLAME_API		operator-(FQuaternion const& l, FQuaternion const& r);