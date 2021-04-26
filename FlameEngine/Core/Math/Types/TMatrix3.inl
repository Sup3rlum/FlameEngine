
template<typename GenType>
MATH_API_INLINE TVector3<GenType> operator*(const TMatrix3<GenType>& l, const TVector3<GenType>& r)
{
	TVector3<GenType> v0 = l[0] * r;
	TVector3<GenType> v1 = l[1] * r;
	TVector3<GenType> v2 = l[2] * r;

	return TVector3<GenType>
	(
		v0.x + v0.y + v0.z,
		v1.x + v1.y + v1.z,
		v2.x + v2.y + v2.z
	);
}
