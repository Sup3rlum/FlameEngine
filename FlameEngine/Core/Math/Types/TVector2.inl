


template<typename GenType>
MATH_API_INLINE bool					operator==(const TVector2<GenType>& l, const TVector2<GenType>& r)
{
	return (
		l.x == r.x &&
		l.y == r.y
		);
}

template<typename GenType>
MATH_API_INLINE bool					operator!=(const TVector2<GenType>& l, const TVector2<GenType>& r)
{
	return !(l == r);
}

template<typename GenType>
MATH_API_INLINE TVector2<GenType>			operator*(const TVector2<GenType>& l, const TVector2<GenType>& r)
{
	return TVector2<GenType>(
		l.x * r.x,
		l.y * r.y
		);
}


template<typename GenType>
MATH_API_INLINE TVector2<GenType>			operator/(const TVector2<GenType>& l, const TVector2<GenType>& r)
{
	return TVector2<GenType>(
		l.x / r.x,
		l.y / r.y
		);
}

template<typename GenType>
MATH_API_INLINE TVector2<GenType>			operator*(const TVector2<GenType>& l, const GenType& _scalar)
{
	return TVector2<GenType>(
		l.x * _scalar,
		l.y * _scalar
		);
}

template<typename GenType>
MATH_API_INLINE TVector2<GenType>			operator/(const TVector2<GenType>& l, const GenType& _scalar)
{
	return TVector2<GenType>(
		l.x / _scalar,
		l.y / _scalar
		);
}




template<typename GenType>
MATH_API_INLINE TVector2<GenType>			operator+(const TVector2<GenType>& l, const TVector2<GenType>& r)
{
	return TVector2<GenType>(
		l.x + r.x,
		l.y + r.y
		);
}

template<typename GenType>
MATH_API_INLINE TVector2<GenType>			operator-(const TVector2<GenType>& l, const TVector2<GenType>& r)
{
	return TVector2<GenType>(
		l.x - r.x,
		l.y - r.y
		);
}