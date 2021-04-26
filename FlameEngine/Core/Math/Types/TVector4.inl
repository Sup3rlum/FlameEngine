


template<typename GenType>
MATH_API_INLINE bool					operator==(const TVector4<GenType>& l, const TVector4<GenType>& r)
{
	return (
		l.x == r.x &&
		l.y == r.y &&
		l.z == r.z &&
		l.w == r.w
		);
}

template<typename GenType>
MATH_API_INLINE bool					operator!=(const TVector4<GenType>& l, const TVector4<GenType>& r)
{
	return !(l == r);
}

template<typename GenType>
MATH_API_INLINE TVector4<GenType>			operator*(const TVector4<GenType>& l, const TVector4<GenType>& r)
{
	return TVector4<GenType>(
		l.x * r.x,
		l.y * r.y,
		l.z * r.z,
		l.w * r.w
	);
}


template<typename GenType>
MATH_API_INLINE TVector4<GenType>			operator/(const TVector4<GenType>& l, const TVector4<GenType>& r)
{
	return TVector4<GenType>(
		l.x / r.x,
		l.y / r.y,
		l.z / r.z,
		l.w / r.w
	);
}

template<typename GenType>
MATH_API_INLINE TVector4<GenType>			operator*(const TVector4<GenType>& l, const GenType& _scalar)
{
	return TVector4<GenType>(
		l.x * _scalar,
		l.y * _scalar,
		l.z * _scalar,
		l.w * _scalar
	);
}

template<typename GenType>
MATH_API_INLINE TVector4<GenType>			operator/(const TVector4<GenType>& l, const GenType& _scalar)
{
	return TVector4<GenType>(
		l.x / _scalar,
		l.y / _scalar,
		l.z / _scalar,
		l.w / _scalar
	);
}




template<typename GenType>
MATH_API_INLINE TVector4<GenType>			operator+(const TVector4<GenType>& l, const TVector4<GenType>& r)
{
	return TVector4<GenType>(
		l.x + r.x,
		l.y + r.y,
		l.z + r.z,
		l.w + r.w
	);
}

template<typename GenType>
MATH_API_INLINE TVector4<GenType>			operator-(const TVector4<GenType>& l, const TVector4<GenType>& r)
{
	return TVector4<GenType>(
		l.x - r.x,
		l.y - r.y,
		l.z - r.z,
		l.w - r.w
	);
}


