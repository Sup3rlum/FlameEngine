#pragma once

#include "Core/Common/CoreCommon.h"
#include "../Geometry/fPlane.h"
#include "FVector4.h"
#include "FMatrix3.h"
#include "FQuaternion.h"

EXPORT(class, FMatrix4)
{
protected:
	FVector4 value[4];

public:

		
	FMatrix4() : FMatrix4(1) {}

	FMatrix4(float s) : 
		value
	{ 
		FVector4(s, 0, 0, 0), 
		FVector4(0, s, 0, 0),
		FVector4(0, 0, s, 0), 
		FVector4(0, 0, 0, s) 
	} {}

	FMatrix4(
		const FVector4& v1,
		const FVector4& v2,
		const FVector4& v3,
		const FVector4& v4) :
		value
	{ 
		FVector4(v1), 
		FVector4(v2), 
		FVector4(v3), 
		FVector4(v4) 
	} {}


	FMatrix4(
		const FMatrix3& m) :
		value
	{
		FVector4(m[0], 0),
		FVector4(m[1], 0),
		FVector4(m[2], 0),
		FVector4(0)
	} {}

	static FMatrix3 ToMatrix3(const FMatrix4& _m);
	static FMatrix4 Transpose(const FMatrix4& _m);

	static FMatrix4 Identity();
	static float Determinant(const FMatrix4& _m);
	static FMatrix4 Inverse(const FMatrix4& _m);


	FVector4& operator[](size_t _index)
	{
		assert(_index < 4);

		return value[_index];
	}


	const FVector4& operator[](size_t _index) const
	{
		assert(_index < 4);

		return value[_index];
	}



	FMatrix4& operator=(const FMatrix4& m)
	{
		this->value[0] = m[0];
		this->value[1] = m[1];
		this->value[2] = m[2];
		this->value[3] = m[3];

		return *this;
	}

};

FLAME_API FMatrix4 operator*(const float& l, const FMatrix4& r);
FLAME_API FMatrix4 operator*(const FMatrix4& l, const float& r);


FLAME_API FVector4	operator*(const FMatrix4& l, FVector4 const& r);
FLAME_API FMatrix4	operator*(const FMatrix4& l, const FMatrix4& r);