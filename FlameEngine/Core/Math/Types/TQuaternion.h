#pragma once

#include "Core/Common/CoreCommon.h"
#include "TVector4.h"
#include "TMatrix3.h"
#include "../Functions/Functions.h"

template<typename GenType>
class  TQuaternion
{
public:
	union
	{
		struct { GenType w, x, y, z; };
		struct { GenType r, i, j, k; };


		struct { GenType w;  TVector2<GenType> xy; GenType z; };
		struct { GenType r;  TVector2<GenType> ij; GenType k; };


		struct { TVector2<GenType> wx; TVector2<GenType> yz; };
		struct { TVector2<GenType> ri; TVector2<GenType> jk; };


		struct { TVector3<GenType> wxy; GenType z; };
		struct { TVector3<GenType> rij; GenType k; };


		struct { GenType w; TVector3<GenType> xyz; };
		struct { GenType r; TVector3<GenType> ijk; };

		struct { TVector4<GenType> vector4; };
		struct { GenType data[4]; };
		struct { __m128 mmv; };
	};
	
	TQuaternion() :
		i(0),
		j(0),
		k(0),
		r(0) {}
	TQuaternion(GenType r, GenType i, GenType j, GenType k) :
		i(i),
		j(j),
		k(k),
		r(r) {}

	TQuaternion(GenType r, const TVector3<GenType>  v) :
		i(v.x),
		j(v.y),
		k(v.z),
		r(r) {}



	TQuaternion(TVector4<GenType>  vec) :
		vector4(vec) {}

	TQuaternion(const TQuaternion& other) :
		r(other.r),
		i(other.i),
		j(other.j),
		k(other.k)
	{
	}


	TQuaternion(const TMatrix3<GenType>& rot)
	{
		//auto rot = TMatrix3<GenType>::Transpose(_rot);
		GenType diagonal = rot.Trace();

		if (diagonal > 0) 
		{
			GenType w4 = (GenType)(FMath::Sqrt(diagonal + GenType(1)) * GenType(2));
			w = w4 / GenType(4);
			x = (rot[1][2] - rot[2][1]) / w4;
			y = (rot[2][0] - rot[0][2]) / w4;
			z = (rot[0][1] - rot[1][0]) / w4;
		}

		else if ((rot[0][0] > rot[1][1]) && (rot[0][0] > rot[2][2])) 
		{
			GenType x4 = (GenType)(FMath::Sqrt(GenType(1) + rot[0][0] - rot[1][1] - rot[2][2]) * GenType(2));
			w = (rot[1][2] - rot[2][1]) / x4;
			x = x4 / GenType(4);
			y = (rot[0][1] + rot[1][0]) / x4;
			z = (rot[0][2] + rot[2][0]) / x4;
		}
		else if (rot[1][1] > rot[2][2]) 
		{
			GenType y4 = (GenType)(FMath::Sqrt(GenType(1) + rot[1][1] - rot[0][0] - rot[2][2]) * GenType(2));
			w = (rot[2][0] - rot[0][2]) / y4;
			x = (rot[0][1] + rot[1][0]) / y4;
			y = y4 / GenType(4);
			z = (rot[1][2] + rot[2][1]) / y4;
		}
		else
		{
			GenType z4 = (GenType)(FMath::Sqrt(GenType(1) + rot[2][2] - rot[0][0] - rot[1][1]) * GenType(2));
			w = (rot[0][1] - rot[1][0]) / z4;
			x = (rot[0][2] + rot[2][0]) / z4;
			y = (rot[1][2] + rot[2][1]) / z4;
			z = z4 / GenType(4);
		}
	}



	GenType Length() const
	{
		return sqrt(LengthSquared());
	}

	GenType LengthSquared() const
	{
		return x * x + y * y + z * z + w * w;
	}




	static TQuaternion Normalize(const TQuaternion& q)
	{
		return q / q.Length();
	}

	static TQuaternion Identity()
	{
		return TQuaternion(1, 0, 0, 0);
	}

	static TQuaternion Inverse(TQuaternion q)
	{
		return TQuaternion(q.r, -q.i, -q.j, -q.j) / q.Length();
	}
	
	static TVector3<GenType> Transform(TQuaternion q, TVector3<GenType> v)
	{
		return v;// q* v;
	}
	static TQuaternion FromEulerAngles(TVector3<GenType> eulerAngles)
	{
		TVector3<GenType> c = FMath::Cos(eulerAngles * GenType(0.5));
		TVector3<GenType> s = FMath::Sin(eulerAngles * GenType(0.5));

		return TQuaternion
		(
			c.x * c.y * c.z + s.x * s.y * s.z,
			s.x * c.y * c.z - c.x * s.y * s.z,
			c.x * s.y * c.z + s.x * c.y * s.z,
			c.x * c.y * s.z - s.x * s.y * c.z
		);
	}
	static TQuaternion FromEulerAngles(GenType yaw, GenType pitch, GenType roll)
	{
		const TVector3<GenType> eulerAngles(yaw, pitch, roll);

		TVector3<GenType> c = FMath::Cos(eulerAngles * GenType(0.5));
		TVector3<GenType> s = FMath::Sin(eulerAngles * GenType(0.5));

		return TQuaternion
		(
			c.x * c.y * c.z + s.x * s.y * s.z,
			s.x * c.y * c.z - c.x * s.y * s.z,
			c.x * s.y * c.z + s.x * c.y * s.z,
			c.x * c.y * s.z - s.x * s.y * c.z
		);
	}


	static TQuaternion FromAxisAngle(GenType angle, TVector3<GenType> axis)
	{
		TVector3<GenType> qv = axis * FMath::Sin(angle / GenType(2));
		GenType qr = FMath::Cos(angle / GenType(2));

		return TQuaternion(qr, qv);
	}


	static TQuaternion Slerp(const TQuaternion& q1, const TQuaternion& q2, GenType weight)
	{
		TQuaternion Result;

		GenType dot = q1.r * q2.r +
			q1.i * q2.i +
			q1.j * q2.j +
			q1.k * q2.k;

		GenType invWeight = GenType(1) - weight;

		if (dot < GenType(0))
		{
			Result = q1 * invWeight - q2 * weight;
		}
		else
		{
			Result = q1 * invWeight + q2 * weight;
		}

		return TQuaternion::Normalize(Result);
	}

	static TQuaternion LookAt(const TVector3<GenType>& origin, const TVector3<GenType>& target)
	{
		TVector3<GenType> dir = TVector3<GenType>::Normalize(target - origin);
		TVector3<GenType> rotAxis = TVector3<GenType>::Normalize(TVector3<GenType>(0, 0, 1) ^ dir);

		float dot = TVector3<GenType>::Dot(TVector3<GenType>(0, 0, 1), dir);
		float ang = std::acosf(dot);

		return TQuaternion::FromAxisAngle(GenType(ang), rotAxis);
	}

	GenType& operator[](size_t _index)
	{
		assert(_index < 4);

		return data[_index];
	}

	const GenType& operator[](size_t _index) const
	{
		assert(_index < 4);

		return data[_index];
	}


	TQuaternion& operator=(const TQuaternion& v)
	{
		this->r = v.r;
		this->i = v.i;
		this->j = v.j;
		this->k = v.k;

		return *this;
	}


};

#include "TQuaternion.inl"
