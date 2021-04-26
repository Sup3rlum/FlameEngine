

template<typename GenType>
MATH_API_INLINE bool					operator==(const TCmplx<GenType>& l, const TCmplx<GenType>& r)
{
	return (
		l.x == r.x &&
		l.y == r.y
		);
}

template<typename GenType>
MATH_API_INLINE bool					operator!=(const TCmplx<GenType>& l, const TCmplx<GenType>& r)
{
	return !(l == r);
}

template<typename GenType>
MATH_API_INLINE TCmplx<GenType>			operator*(const TCmplx<GenType>& l, const TCmplx<GenType>& r)
{
	return TCmplx<GenType>(
		l.x * r.x - l.y * r.y,
		l.y * r.x + l.x * r.y
		);
}


template<typename GenType>
MATH_API_INLINE TCmplx<GenType>			operator/(const TCmplx<GenType>& l, const TCmplx<GenType>& r)
{
	return TCmplx<GenType>(
		(l.x * r.x + l.y * r.y) / r.MagnitudeSquared(),
		(l.y * r.x - l.x * r.y) / r.MagnitudeSquared()
		);
}

template<typename GenType>
MATH_API_INLINE TCmplx<GenType>			operator*(const TCmplx<GenType>& l, const GenType& _scalar)
{
	return TCmplx<GenType>(
		l.x * _scalar,
		l.y * _scalar
		);
}

template<typename GenType>
MATH_API_INLINE TCmplx<GenType>			operator/(const TCmplx<GenType>& l, const GenType& _scalar)
{
	return TCmplx<GenType>(
		l.x / _scalar,
		l.y / _scalar
		);
}




template<typename GenType>
MATH_API_INLINE TCmplx<GenType>			operator+(const TCmplx<GenType>& l, const TCmplx<GenType>& r)
{
	return TCmplx<GenType>(
		l.x + r.x,
		l.y + r.y
		);
}

template<typename GenType>
MATH_API_INLINE TCmplx<GenType>			operator-(const TCmplx<GenType>& l, const TCmplx<GenType>& r)
{
	return TCmplx<GenType>(
		l.x - r.x,
		l.y - r.y
		);
}