#pragma once


#include "simddef.h"

template<>
MATH_API_INLINE TMatrix4<float> operator*(const TMatrix4<float>& a, const TMatrix4<float>& b)
{
    TMatrix4<float> Result(0);

    const auto row1 = b[0].mmv;
    const auto row2 = b[1].mmv;
    const auto row3 = b[2].mmv;
    const auto row4 = b[3].mmv;

    for (int i = 0; i < 4; i++)
    {
        __m128 brod1 = _mm_set1_ps(a[i][0]);
        __m128 brod2 = _mm_set1_ps(a[i][1]);
        __m128 brod3 = _mm_set1_ps(a[i][2]);
        __m128 brod4 = _mm_set1_ps(a[i][3]);

        Result[i].mmv = _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(brod1, row1),
                _mm_mul_ps(brod2, row2)),
            _mm_add_ps(
                _mm_mul_ps(brod3, row3),
                _mm_mul_ps(brod4, row4)));
    }

    return Result;
}


template<>
MATH_API_INLINE TVector4<float> operator*(TMatrix4<float> const& mat, TVector4<float> const& vec)
{
    __m128 p1 = _mm_dp_ps(vec.mmv, mat[0].mmv, 0xff);
    __m128 p2 = _mm_dp_ps(vec.mmv, mat[1].mmv, 0xff);
    __m128 p3 = _mm_dp_ps(vec.mmv, mat[2].mmv, 0xff);
    __m128 p4 = _mm_dp_ps(vec.mmv, mat[3].mmv, 0xff);

    return TVector4<float>(
        p1.m128_f32[0], 
        p2.m128_f32[0],
        p3.m128_f32[0],
        p4.m128_f32[0]
    );
}