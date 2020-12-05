#pragma once

#include "../../dll/nchfx.h"

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

	constexpr FMatrix3(float s) :
		value
	{
		FVector3(s, 0, 0),
		FVector3(0, s, 0),
		FVector3(0, 0, s)
	} {}

	constexpr FMatrix3(
		FVector3 const& v1,
		FVector3 const& v2,
		FVector3 const& v3) :
		value
	{
		FVector3(v1),
		FVector3(v2),
		FVector3(v3)
	} {}



	static FMatrix3 Transpose(FMatrix3 m);


	FVector3& operator[](FL_INT32 _index)
	{
		assert(_index < 3);

		return value[_index];
	}


	constexpr FVector3 const& operator[](FL_INT32 _index) const
	{
		assert(_index < 3);

		return value[_index];
	}



	constexpr FMatrix3& operator=(FMatrix3 const& m)
	{
		this->value[0] = m[0];
		this->value[1] = m[1];
		this->value[2] = m[2];

		return *this;
	}


};

FLAME_DLL FVector3	 operator*(FMatrix3 const& l, FVector3 const& r);