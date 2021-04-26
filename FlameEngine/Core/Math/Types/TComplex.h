#pragma once


#include "Core/Common/CoreCommon.h"
#include "../MathBase.h"

template<typename TField>
struct TCmplx
{

	union
	{
		struct { TField Real, Imaginary; };
		struct { TField x, y; };
		struct { TField r, i; };
		struct { TField data[2]; };
	};
	TCmplx() :
		Real(0),
		Imaginary(0) {}

	TCmplx(TField s) :
		Real(s),
		Imaginary(0) {}

	TCmplx(TField r, TField i) :
		Real(r),
		Imaginary(i) {}

	TField MagnitudeSquared() const
	{
		return Real * Real + Imaginary * Imaginary;
	}
	TField Magnitude() const
	{
		return sqrt(Real * Real + Imaginary * Imaginary);
	}

	static TCmplx Conjugate(TCmplx c)
	{
		return TCmplx(c.Real, -c.Imaginary);
	}

	TField& operator[](size_t _index)
	{
		assert(_index < 2);

		return data[_index];
	}
	const TField& operator[](size_t _index) const
	{
		assert(_index < 2);

		return data[_index];
	}


	TCmplx& operator=(const TCmplx& v)
	{
		this->x = v.x;
		this->y = v.y;

		return *this;
	}


};


#include "TComplex.inl"



typedef TCmplx<float> FCmplx;
typedef TCmplx<int> ICmplx;
typedef TCmplx<double> DCmplx;
typedef TCmplx<long long> LCmplx;