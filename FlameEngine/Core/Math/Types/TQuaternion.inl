
template<typename GenType>
FORCEINLINE bool operator==(TQuaternion<GenType> const& l, TQuaternion<GenType> const& r)
{
	return (
		l.r == r.r &&
		l.i == r.i &&
		l.j == r.j &&
		l.k == r.k
		);
}

template<typename GenType>
FORCEINLINE bool operator!=(TQuaternion<GenType> const& l, TQuaternion<GenType> const& r)
{
	return !(l == r);
}

template<typename GenType>
FORCEINLINE TQuaternion<GenType>			operator*(TQuaternion<GenType> const& l, TQuaternion<GenType> const& r)
{
	return TQuaternion<GenType>(
		(l.r * r.r) - (l.ijk & r.ijk),
		l.ijk ^ r.ijk + r.ijk * l.r + l.ijk * r.r
	);
}


template<typename GenType>
FORCEINLINE TQuaternion<GenType>			operator*(TQuaternion<GenType> const& q, TVector3<GenType> const& v)
{
	TVector3<GenType> uv = q.ijk ^ v;
	TVector3<GenType> uuv = uv ^ v;

	return v + ((uv * q.r) + uuv) * static_cast<GenType>(2);
}




template<typename GenType>
FORCEINLINE TQuaternion<GenType>			operator*(TQuaternion<GenType> const& l, float const& scalar)
{
	return TQuaternion<GenType>(
		l.r * scalar,
		l.i * scalar,
		l.j * scalar,
		l.k * scalar
	);
}

template<typename GenType>
FORCEINLINE TQuaternion<GenType>			operator/(TQuaternion<GenType> const& l, float const& scalar)
{
	return TQuaternion<GenType>(
		l.r / scalar,
		l.i / scalar,
		l.j / scalar,
		l.k / scalar
	);
}


template<typename GenType>
FORCEINLINE TQuaternion<GenType>			operator+(TQuaternion<GenType> const& l, TQuaternion<GenType> const& r)
{
	return TQuaternion<GenType>(
		l.r + r.r,
		l.i + r.i,
		l.j + r.j,
		l.k + r.k
	);
}


template<typename GenType>
FORCEINLINE TQuaternion<GenType>			operator-(TQuaternion<GenType> const& l, TQuaternion<GenType> const& r)
{
	return TQuaternion<GenType>(
		l.r - r.r,
		l.i - r.i,
		l.j - r.j,
		l.k - r.k
	);
}

