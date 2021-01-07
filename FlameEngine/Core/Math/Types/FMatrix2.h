#pragma once


#include "Core/Common/CoreCommon.h"

#include "FVector2.h"

EXPORT(class, FMatrix2)
{
	FVector2 value[2];
public:



	constexpr FMatrix2() :
		value
	{
		FVector2(0),
		FVector2(0)
	} {}

	FMatrix2(float s) :
		value
	{
		FVector2(s, 0),
		FVector2(0, s)
	} {}

	FMatrix2(
		const FVector2& v1,
		const FVector2& v2) :
		value
	{
		FVector2(v1),
		FVector2(v2)
	} {}



	static FMatrix2 Transpose(const FMatrix2& m);
	static FMatrix2 Inverse(const FMatrix2& m);
	static float Determinant(const FMatrix2& m);


	FVector2& operator[](size_t _index)
	{
		assert(_index < 2);

		return value[_index];
	}


	const FVector2& operator[](size_t _index) const
	{
		assert(_index < 2);

		return value[_index];
	}



	FMatrix2& operator=(FMatrix2 const& m)
	{
		this->value[0] = m[0];
		this->value[1] = m[1];

		return *this;
	}

};

FLAME_API FVector2	 operator*(const FMatrix2& l, const FVector2& r);
FLAME_API FMatrix2	 operator*(const float& l, const FMatrix2& r);