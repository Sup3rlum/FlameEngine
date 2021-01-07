#pragma once

#include "Core/Common/CoreCommon.h"

#include "FVector3.h"

EXPORT(class,  FMatrix3)
{

		FVector3 value[3];
public:



	constexpr FMatrix3() :
		value
	{
		FVector3(0),
		FVector3(0),
		FVector3(0)
	} {}

	FMatrix3(float s) :
		value
	{
		FVector3(s, 0, 0),
		FVector3(0, s, 0),
		FVector3(0, 0, s)
	} {}

	FMatrix3(
		FVector3 const& v1,
		FVector3 const& v2,
		FVector3 const& v3) :
		value
	{
		FVector3(v1),
		FVector3(v2),
		FVector3(v3)
	} {}



	static FMatrix3 Transpose(const FMatrix3& m);
	static FMatrix3 Inverse(const FMatrix3& m);
	static float Determinant(const FMatrix3& m);

	FVector3& operator[](size_t _index)
	{
		assert(_index < 3);

		return value[_index];
	}


	const FVector3& operator[](size_t _index) const
	{
		assert(_index < 3);

		return value[_index];
	}



	FMatrix3& operator=(const FMatrix3& m)
	{
		this->value[0] = m[0];
		this->value[1] = m[1];
		this->value[2] = m[2];

		return *this;
	}


};

FLAME_API FVector3	 operator*(const FMatrix3& l, FVector3 const& r);