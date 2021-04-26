


template<typename GenType>
MATH_API_INLINE bool					operator==(const TVector3<GenType>& l, const TVector3<GenType>& r)
{
	return (
		l.x == r.x &&
		l.y == r.y &&
		l.z == r.z
		);
}

template<typename GenType>
MATH_API_INLINE bool					operator!=(const TVector3<GenType>& l, const TVector3<GenType>& r)
{
	return !(l == r);
}

template<typename GenType>
MATH_API_INLINE TVector3<GenType>			operator*(const TVector3<GenType>& l, const TVector3<GenType>& r)
{
	return TVector3<GenType>(
		l.x * r.x,
		l.y * r.y,
		l.z * r.z
		);
}


template<typename GenType>
MATH_API_INLINE TVector3<GenType>			operator/(const TVector3<GenType>& l, const TVector3<GenType>& r)
{
	return TVector3<GenType>(
		l.x / r.x,
		l.y / r.y,
		l.z / r.z
		);
}

template<typename GenType>
MATH_API_INLINE TVector3<GenType>			operator*(const TVector3<GenType>& l, const GenType& _scalar)
{
	return TVector3<GenType>(
		l.x * _scalar,
		l.y * _scalar,
		l.z * _scalar
		);
}

template<typename GenType>
MATH_API_INLINE TVector3<GenType>			operator/(const TVector3<GenType>& l, const GenType& _scalar)
{
	return TVector3<GenType>(
		l.x / _scalar,
		l.y / _scalar,
		l.z / _scalar
		);
}




template<typename GenType>
MATH_API_INLINE TVector3<GenType>			operator+(const TVector3<GenType>& l, const TVector3<GenType>& r)
{
	return TVector3<GenType>(
		l.x + r.x,
		l.y + r.y,
		l.z + r.z
		);
}

template<typename GenType>
MATH_API_INLINE TVector3<GenType>			operator-(const TVector3<GenType>& l, const TVector3<GenType>& r)
{
	return TVector3<GenType>(
		l.x - r.x,
		l.y - r.y,
		l.z - r.z
		);
}



template<typename GenType>
MATH_API_INLINE TVector3<GenType>			operator+(const TVector3<GenType>& l, GenType r)
{
	return TVector3<GenType>(
		l.x + r,
		l.y + r,
		l.z + r
		);
}

template<typename GenType>
MATH_API_INLINE TVector3<GenType>			operator-(const TVector3<GenType>& l, GenType r)
{
	return TVector3<GenType>(
		l.x - r,
		l.y - r,
		l.z - r
		);
}




template<typename GenType>
MATH_API_INLINE TVector3<GenType>			operator^(const TVector3<GenType>& l, const TVector3<GenType>& r)
{
	return TVector3<GenType>::Cross(l, r);
}
template<typename GenType>
MATH_API_INLINE GenType			operator&(const TVector3<GenType>& l, const TVector3<GenType>& r)
{
	return TVector3<GenType>::Dot(l, r);
}