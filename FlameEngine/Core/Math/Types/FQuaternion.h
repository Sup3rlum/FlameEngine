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
		struct { float data[4]; };
		struct { __m128 mmv; };
	};
	
	constexpr FQuaternion() :
		i(0),
		j(0),
		k(0),
		r(0) {}
	FQuaternion(float _r, float _i, float _j, float _k) :
		i(_i),
		j(_j),
		k(_k),
		r(_r) {}

	FQuaternion(float _r, FVector3 _v) :
		i(_v.x),
		j(_v.y),
		k(_v.z),
		r(_r) {}



	FQuaternion(FVector4 _vec) :
		vector4(_vec) {}




	float Length();
	float LengthSquared();



	static FQuaternion Normalize(FQuaternion q);
	static FQuaternion Identity();
	static const FQuaternion& Inverse(FQuaternion q);





	float& operator[](size_t _index)
	{
		assert(_index < 4);

		return data[_index];
	}

	const float& operator[](size_t _index) const
	{
		assert(_index < 4);

		return data[_index];
	}


	FQuaternion& operator=(const FQuaternion& v)
	{
		this->r = v.r;
		this->i = v.i;
		this->j = v.j;
		this->k = v.k;

		return *this;
	}


};


bool		FLAME_API			operator==(const FQuaternion& l, const FQuaternion& r);
bool		FLAME_API			operator!=(const FQuaternion& l, const FQuaternion& r);

FQuaternion	FLAME_API		operator*(const FQuaternion& l, const FQuaternion& r);
FQuaternion	FLAME_API		operator*(const FQuaternion& l, const float& _scalar);
FQuaternion	FLAME_API		operator/(const FQuaternion& l, const float& _scalar);
FQuaternion	FLAME_API		operator+(const FQuaternion& l, const FQuaternion& r);
FQuaternion	FLAME_API		operator-(const FQuaternion& l, const FQuaternion& r);