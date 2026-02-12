

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


template<typename GenType>
MATH_API_INLINE TVector4<GenType> operator*(TMatrix4<GenType> const& l, TVector4<GenType> const& r)
{
	GenType _v0 = TVector4<GenType>::Dot(l[0], r);
	GenType _v1 = TVector4<GenType>::Dot(l[1], r);
	GenType _v2 = TVector4<GenType>::Dot(l[2], r);
	GenType _v3 = TVector4<GenType>::Dot(l[3], r);

	return TVector4<GenType>
	(
		_v0,
		_v1,
		_v2,
		_v3
	);
}


// Multiplication is operator*(B, A) -> A^T*B

template<typename GenType>
MATH_API_INLINE TMatrix4<GenType> operator*(TMatrix4<GenType> const& l, TMatrix4<GenType> const& r)
{

	const auto A = TMatrix4<GenType>::Transpose(l);
	const auto B = TMatrix4<GenType>::Transpose(r);

	const TVector4<GenType>& SrcA0 = A[0];
	const TVector4<GenType>& SrcA1 = A[1];
	const TVector4<GenType>& SrcA2 = A[2];
	const TVector4<GenType>& SrcA3 = A[3];
		  
	const TVector4<GenType>& SrcB0 = B[0];
	const TVector4<GenType>& SrcB1 = B[1];
	const TVector4<GenType>& SrcB2 = B[2];
	const TVector4<GenType>& SrcB3 = B[3];

	TMatrix4<GenType> m;
	m[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
	m[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
	m[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
	m[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];

	return m;
}
