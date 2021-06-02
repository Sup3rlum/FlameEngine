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
		GenType diagonal = rot.Trace();

		if (diagonal > 0) 
		{
			GenType w4 = (GenType)(FMathFunc::Sqrt(diagonal + GenType(1)) * GenType(2));
			w = w4 / GenType(4);
			x = (rot[2][1] - rot[1][2]) / w4;
			y = (rot[0][2] - rot[2][0]) / w4;
			z = (rot[1][0] - rot[0][1]) / w4;
		}

		else if ((rot[0][0] > rot[1][1]) && (rot[0][0] > rot[2][2])) 
		{
			GenType x4 = (GenType)(FMathFunc::Sqrt(GenType(1) + rot[0][0] - rot[1][1] - rot[2][2]) * GenType(2));
			w = (rot[2][1] - rot[1][2]) / x4;
			x = x4 / GenType(4);
			y = (rot[0][1] + rot[1][0]) / x4;
			z = (rot[0][2] + rot[2][0]) / x4;
		}
		else if (rot[1][1] > rot[2][2]) 
		{
			GenType y4 = (GenType)(FMathFunc::Sqrt(GenType(1) + rot[1][1] - rot[0][0] - rot[2][2]) * GenType(2));
			w = (rot[0][2] - rot[2][0]) / y4;
			x = (rot[0][1] + rot[1][0]) / y4;
			y = y4 / GenType(4);
			z = (rot[1][2] + rot[2][1]) / y4;
		}
		else
		{
			GenType z4 = (GenType)(FMathFunc::Sqrt(GenType(1) + rot[2][2] - rot[0][0] - rot[1][1]) * GenType(2));
			w = (rot[1][0] - rot[0][1]) / z4;
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
		TVector3<GenType> c = FMathFunc::Cos(eulerAngles * GenType(0.5));
		TVector3<GenType> s = FMathFunc::Sin(eulerAngles * GenType(0.5));

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
		TVector3<GenType> qv = axis * FMathFunc::Sin(angle / GenType(2));
		GenType qr = FMathFunc::Cos(angle / GenType(2));

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

	/*
	static TQuaternion FromOrthoBasis(TVector3<GenType> forward, TVector3<GenType> right, TVector3<GenType> up)
	{
		
		GenType trace = right.x + up.y + forward.z;
		if (trace > 0.0) {
			double s = 0.5 / sqrt(trace + 1.0);
			q.w = 0.25 / s;
			q.x = (U.z - F.y) * s;
			q.y = (F.x - R.z) * s;
			q.z = (R.y - U.x) * s;
		}
		else {
			if (R.x > U.y && R.x > F.z) {
				double s = 2.0 * sqrt(1.0 + R.x - U.y - F.z);
				q.w = (U.z - F.y) / s;
				q.x = 0.25 * s;
				q.y = (U.x + R.y) / s;
				q.z = (F.x + R.z) / s;
			}
			else if (U.y > F.z) {
				double s = 2.0 * sqrt(1.0 + U.y - R.x - F.z);
				q.w = (F.x - R.z) / s;
				q.x = (U.x + R.y) / s;
				q.y = 0.25 * s;
				q.z = (F.y + U.z) / s;
			}
			else {
				double s = 2.0 * sqrt(1.0 + F.z - R.x - U.y);
				q.w = (R.y - U.x) / s;
				q.x = (F.x + R.z) / s;
				q.y = (F.y + U.z) / s;
				q.z = 0.25 * s;
			}
		}
	}*/

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
