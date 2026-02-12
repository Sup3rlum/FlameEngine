#include "simddef.h"



template<>
MATH_API_INLINE TVector4<float>	operator*(const TVector4<float>& l, const TVector4<float>& r)
{
	return _mm_mul_ps(l.mmv, r.mmv);
}


template<>
MATH_API_INLINE TVector4<float> operator/(const TVector4<float>& l, const TVector4<float>& r)
{
	return _mm_div_ps(l.mmv, r.mmv);
}

template<>
MATH_API_INLINE TVector4<float>	operator*(const TVector4<float>& l, const float& _scalar)
{
	__m128 broad = _mm_set_ps1(_scalar);
	return _mm_mul_ps(l.mmv, broad);
}

template<>
MATH_API_INLINE TVector4<float>	operator/(const TVector4<float>& l, const float& _scalar)
{
	__m128 broad = _mm_set_ps1(_scalar);
	return _mm_div_ps(l.mmv, broad);
}


template<>
MATH_API_INLINE TVector4<float> operator+(const TVector4<float>& l, const TVector4<float>& r)
{
	return _mm_add_ps(l.mmv, r.mmv);
}

template<>
MATH_API_INLINE TVector4<float> operator-(const TVector4<float>& l, const TVector4<float>& r)
{
	return _mm_sub_ps(l.mmv, r.mmv);
}


