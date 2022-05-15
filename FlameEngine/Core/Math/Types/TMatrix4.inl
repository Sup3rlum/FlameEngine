

template<typename GenType>
MATH_API_INLINE TMatrix4<GenType> operator*(float const& l, TMatrix4<GenType> const& r)
{

	return TMatrix4<GenType>
	(
		r[0] * l,
		r[1] * l,
		r[2] * l,
		r[3] * l
	);
}


template<typename GenType>
MATH_API_INLINE TMatrix4<GenType> operator*(TMatrix4<GenType> const& l, float const& r)
{

	return TMatrix4<GenType>
	(
		l[0] * r,
		l[1] * r,
		l[2] * r,
		l[3] * r
	);
}


// Multiplication is A^T*V


template<typename GenType>
MATH_API_INLINE TVector4<GenType> operator*(TMatrix4<GenType> const& l, FVector4 const& r)
{
	TVector4<GenType> _v0 = _mm_mul_ps(l[0].mmv, r.mmv);
	TVector4<GenType> _v1 = _mm_mul_ps(l[1].mmv, r.mmv);
	TVector4<GenType> _v2 = _mm_mul_ps(l[2].mmv, r.mmv);
	TVector4<GenType> _v3 = _mm_mul_ps(l[3].mmv, r.mmv);

	return TVector4<GenType>
	(
		_v0.x + _v0.y + _v0.z + _v0.w,
		_v1.x + _v1.y + _v1.z + _v1.w,
		_v2.x + _v2.y + _v2.z + _v2.w,
		_v3.x + _v3.y + _v3.z + _v3.w
	);
}


// Multiplication is operator*(B, A) -> A^T*B

template<typename GenType>
MATH_API_INLINE TMatrix4<GenType> operator*(TMatrix4<GenType> const& l, TMatrix4<GenType> const& r)
{
	const TVector4<GenType>& SrcA0 = l[0];
	const TVector4<GenType>& SrcA1 = l[1];
	const TVector4<GenType>& SrcA2 = l[2];
	const TVector4<GenType>& SrcA3 = l[3];
		  
	const TVector4<GenType>& SrcB0 = r[0];
	const TVector4<GenType>& SrcB1 = r[1];
	const TVector4<GenType>& SrcB2 = r[2];
	const TVector4<GenType>& SrcB3 = r[3];

	TMatrix4<GenType> m;
	m[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
	m[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
	m[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
	m[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];

	return m;
}

