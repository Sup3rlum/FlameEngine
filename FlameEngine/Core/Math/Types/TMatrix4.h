#pragma once

#include "Core/Common/CoreCommon.h"
#include "../Geometry/fPlane.h"
#include "TVector4.h"
#include "TMatrix3.h"


template<typename GenType>
class TMatrix4
{
protected:

	typedef TVector4<GenType> RowType;


	RowType value[4];

public:

		
	TMatrix4() : TMatrix4(1) {}

	TMatrix4(GenType s) : 
		value
	{ 
		RowType(s, 0, 0, 0), 
		RowType(0, s, 0, 0),
		RowType(0, 0, s, 0), 
		RowType(0, 0, 0, s) 
	} {}

	TMatrix4(
		const RowType& v1,
		const RowType& v2,
		const RowType& v3,
		const RowType& v4) :
		value
	{ 
		RowType(v1), 
		RowType(v2), 
		RowType(v3), 
		RowType(v4) 
	} {}


	TMatrix4(
		const TMatrix3<GenType>& m) :
		value
	{
		RowType(m[0], 0),
		RowType(m[1], 0),
		RowType(m[2], 0),
		RowType(0)
	} {}

	static TMatrix3<GenType> ToMatrix3(const TMatrix4& m)
	{
		return TMatrix3<GenType>
		(
			m[0].xyz,
			m[1].xyz,
			m[2].xyz
		);
	}

	static TMatrix4 Transpose(const TMatrix4& m)
	{
		TMatrix4 Result = m;

		Result[0][1] = m[1][0];
		Result[0][2] = m[2][0];

		Result[1][2] = m[2][1];
		Result[2][1] = m[1][2];

		Result[2][0] = m[0][2];
		Result[1][0] = m[0][1];


		Result[0][3] = m[3][0];
		Result[1][3] = m[3][1];
		Result[2][3] = m[3][2];

		Result[3][0] = m[0][3];
		Result[3][1] = m[1][3];
		Result[3][2] = m[2][3];

		return Result;
	}

	static TMatrix4 Identity()
	{
		return TMatrix4
		(
			RowType(1, 0, 0, 0),
			RowType(0, 1, 0, 0),
			RowType(0, 0, 1, 0),
			RowType(0, 0, 0, 1)
		);
	}
	static GenType Determinant(const TMatrix4& m)
	{
		return
			m[0][3] * m[1][2] * m[2][1] * m[3][0] - m[0][2] * m[1][3] * m[2][1] * m[3][0] -
			m[0][3] * m[1][1] * m[2][2] * m[3][0] + m[0][1] * m[1][3] * m[2][2] * m[3][0] +
			m[0][2] * m[1][1] * m[2][3] * m[3][0] - m[0][1] * m[1][2] * m[2][3] * m[3][0] -
			m[0][3] * m[1][2] * m[2][0] * m[3][1] + m[0][2] * m[1][3] * m[2][0] * m[3][1] +
			m[0][3] * m[1][0] * m[2][2] * m[3][1] - m[0][0] * m[1][3] * m[2][2] * m[3][1] -
			m[0][2] * m[1][0] * m[2][3] * m[3][1] + m[0][0] * m[1][2] * m[2][3] * m[3][1] +
			m[0][3] * m[1][1] * m[2][0] * m[3][2] - m[0][1] * m[1][3] * m[2][0] * m[3][2] -
			m[0][3] * m[1][0] * m[2][1] * m[3][2] + m[0][0] * m[1][3] * m[2][1] * m[3][2] +
			m[0][1] * m[1][0] * m[2][3] * m[3][2] - m[0][0] * m[1][1] * m[2][3] * m[3][2] -
			m[0][2] * m[1][1] * m[2][0] * m[3][3] + m[0][1] * m[1][2] * m[2][0] * m[3][3] +
			m[0][2] * m[1][0] * m[2][1] * m[3][3] - m[0][0] * m[1][2] * m[2][1] * m[3][3] -
			m[0][1] * m[1][0] * m[2][2] * m[3][3] + m[0][0] * m[1][1] * m[2][2] * m[3][3];
	}

	static TMatrix4 Inverse(const TMatrix4& m)
	{
		GenType Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		GenType Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		GenType Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

		GenType Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		GenType Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		GenType Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

		GenType Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		GenType Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		GenType Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		GenType Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		GenType Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		GenType Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		GenType Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		GenType Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		GenType Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		GenType Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		GenType Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		GenType Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		RowType Fac0(Coef00, Coef00, Coef02, Coef03);
		RowType Fac1(Coef04, Coef04, Coef06, Coef07);
		RowType Fac2(Coef08, Coef08, Coef10, Coef11);
		RowType Fac3(Coef12, Coef12, Coef14, Coef15);
		RowType Fac4(Coef16, Coef16, Coef18, Coef19);
		RowType Fac5(Coef20, Coef20, Coef22, Coef23);

		RowType Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
		RowType Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
		RowType Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
		RowType Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

		RowType Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
		RowType Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
		RowType Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
		RowType Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

		RowType SignA(+1, -1, +1, -1);
		RowType SignB(-1, +1, -1, +1);

		TMatrix4 Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

		RowType Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

		RowType  Dot0(m[0] * Row0);
		GenType Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

		GenType OneOverDeterminant = 1.0f / Dot1;

		return Inverse * OneOverDeterminant;
	}


	RowType& operator[](size_t _index)
	{
		assert(_index < 4);

		return value[_index];
	}


	const RowType& operator[](size_t _index) const
	{
		assert(_index < 4);

		return value[_index];
	}



	TMatrix4& operator=(const TMatrix4& m)
	{
		this->value[0] = m[0];
		this->value[1] = m[1];
		this->value[2] = m[2];
		this->value[3] = m[3];

		return *this;
	}


	static TMatrix4 Lerp(const TMatrix4& first, const TMatrix4& second, GenType value)
	{
		return TMatrix4(
			RowType::Lerp(first[0], second[0], value),
			RowType::Lerp(first[1], second[1], value),
			RowType::Lerp(first[2], second[2], value),
			RowType::Lerp(first[3], second[3], value)
		);
	}
};


#include "TMatrix4.inl"
