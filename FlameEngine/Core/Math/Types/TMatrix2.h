#pragma once


#include "Core/Common/CoreCommon.h"

#include "TVector2.h"

template<typename GenType>
class TMatrix2
{

	typedef TVector2<GenType> RowType;

	RowType value[2];
public:



	TMatrix2() :
		value
	{
		RowType(0),
		RowType(0)
	} {}

	TMatrix2(GenType s) :
		value
	{
		RowType(s, 0),
		RowType(0, s)
	} {}

	TMatrix2(
		const RowType& v1,
		const RowType& v2) :
		value
	{
		RowType(v1),
		RowType(v2)
	} {}



	static TMatrix2 Transpose(const TMatrix2& m)
	{
		TMatrix2 Result = m;

		Result[0][1] = m[1][0];
		Result[1][0] = m[0][1];

		return Result;
	}
	static TMatrix2 Inverse(const TMatrix2& m)
	{
		TMatrix2 Result = m;

		Result[0][0] = m[1][1];
		Result[1][1] = m[0][0];

		Result[0][1] = -m[0][1];
		Result[1][0] = -m[1][0];

		float invdet = 1.0f / Determinant(m);

		return invdet * Result;
	}
	static GenType Determinant(const TMatrix2& m)
	{
		return m[0][0] * m[1][1] - m[0][1] * m[1][0];
	}



	RowType& operator[](size_t _index)
	{
		assert(_index < 2);

		return value[_index];
	}


	const RowType& operator[](size_t _index) const
	{
		assert(_index < 2);

		return value[_index];
	}



	TMatrix2& operator=(TMatrix2 const& m)
	{
		this->value[0] = m[0];
		this->value[1] = m[1];

		return *this;
	}

};


#include "TMatrix2.inl"
