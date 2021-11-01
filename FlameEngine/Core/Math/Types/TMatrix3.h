#pragma once

#include "Core/Common/CoreCommon.h"

#include "TMatrix2.h"
#include "TVector3.h"



template<typename GenType>
class  TMatrix3
{

	typedef TVector3<GenType> RowType;

	RowType value[3];
public:



	TMatrix3() :
		value
	{
		RowType(0),
		RowType(0),
		RowType(0)
	} {}

	TMatrix3(GenType s) :
		value
	{
		RowType(s, 0, 0),
		RowType(0, s, 0),
		RowType(0, 0, s)
	} {}

	TMatrix3(
		const RowType& v1,
		const RowType& v2,
		const RowType& v3) :
		value
	{
		RowType(v1),
		RowType(v2),
		RowType(v3)
	} {}



	static TMatrix3 Transpose(const TMatrix3& m)
	{
		TMatrix3 Result = m;

		Result[0][1] = m[1][0];
		Result[0][2] = m[2][0];

		Result[1][2] = m[2][1];
		Result[2][1] = m[1][2];

		Result[2][0] = m[0][2];
		Result[1][0] = m[0][1];

		return Result;
	}
	static TMatrix3 Inverse(const TMatrix3& m)
	{

	}
	static GenType Determinant(const TMatrix3& m)
	{

	}


	RowType& operator[](size_t _index)
	{
		assert(_index < 3);

		return value[_index];
	}


	const RowType& operator[](size_t _index) const
	{
		assert(_index < 3);

		return value[_index];
	}



	TMatrix3& operator=(const TMatrix3& m)
	{
		this->value[0] = m[0];
		this->value[1] = m[1];
		this->value[2] = m[2];

		return *this;
	}


	FORCEINLINE GenType Trace() const
	{
		return value[0][0] + value[1][1] + value[2][2];
	}

};

#include "TMatrix3.inl"