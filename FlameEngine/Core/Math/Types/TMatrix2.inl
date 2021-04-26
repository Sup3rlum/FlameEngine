

template<typename GenType>
MATH_API_INLINE TVector2<GenType> operator*(const TMatrix2<GenType>& l, const TVector2<GenType>& r)
{
	TVector2<GenType> _v0 = l[0] * r;
	TVector2<GenType> _v1 = l[1] * r;

	return TVector2<GenType>
	(
		_v0.x + _v0.y,
		_v1.x + _v1.y
	);
}

template<typename GenType>
MATH_API_INLINE TMatrix2<GenType> operator*(GenType l, const TMatrix2<GenType>& r)
{
	return TMatrix2<GenType>
	(
		r[0] * l,
		r[1] * l
	);
}
