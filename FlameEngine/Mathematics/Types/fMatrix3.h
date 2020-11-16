#pragma once

#include "../../dll/nchfx.h"

#include "fVector3.h"

EXPORT_CLASS fMatrix3
{

		fVector3 value[3];
public:



	constexpr fMatrix3() :
		value
	{
		fVector3(0),
		fVector3(0),
		fVector3(0)
	} {}

	constexpr fMatrix3(float s) :
		value
	{
		fVector3(s, 0, 0),
		fVector3(0, s, 0),
		fVector3(0, 0, s)
	} {}

	constexpr fMatrix3(
		fVector3 const& v1,
		fVector3 const& v2,
		fVector3 const& v3) :
		value
	{
		fVector3(v1),
		fVector3(v2),
		fVector3(v3)
	} {}



	static fMatrix3 Transpose(fMatrix3 m);


	fVector3& operator[](FL_INT32 _index)
	{
		assert(_index < 3);

		return value[_index];
	}


	constexpr fVector3 const& operator[](FL_INT32 _index) const
	{
		assert(_index < 3);

		return value[_index];
	}



	constexpr fMatrix3& operator=(fMatrix3 const& m)
	{
		this->value[0] = m[0];
		this->value[1] = m[1];
		this->value[2] = m[2];

		return *this;
	}


};

