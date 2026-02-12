#pragma once


#include <immintrin.h>

template<typename _Type>
struct _FMMVec4Type
{
	typedef _Type _MmType;
};

template<>
struct _FMMVec4Type<int>
{
	typedef __m128i _MmType;
};

template<>
struct _FMMVec4Type<float>
{
	typedef __m128 _MmType;
};

template<>
struct _FMMVec4Type<double>
{
	typedef __m256 _MmType;
};


template<typename _Type>
using FMMVec4Type = _FMMVec4Type<_Type>::_MmType;