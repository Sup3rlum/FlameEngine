#pragma once


#include "Core/Common/CoreCommon.h"
#include "TVector2.h"


template<typename GenType>
class TVector3
{
public:
	
	union
	{
		struct { GenType x, y, z; };
		struct { GenType r, g, b; };
		struct { GenType s, t, p; };
		struct { TVector2<GenType> xy; GenType z; };
		struct { TVector2<GenType> rg; GenType b; };
		struct { TVector2<GenType> st; GenType p; };
		struct { GenType x;  TVector2<GenType> yz; };
		struct { GenType r;  TVector2<GenType> gb; };
		struct { GenType s;  TVector2<GenType> tp; };
		struct { GenType data[3]; };
	};


	TVector3() : 
		x(0), 
		y(0), 
		z(0) {}

	TVector3(GenType v) : 
		x(v), 
		y(v), 
		z(v) {}
	TVector3(GenType x, GenType y, GenType z) : 
		x(x), 
		y(y), 
		z(z) {}
	TVector3(TVector2<GenType> vec, GenType z) :
		x(vec.x), 
		y(vec.y), 
		z(z) {}


	template<typename OtherType>
	TVector3(const TVector3<OtherType> other) :
		x(other.x),
		y(other.y),
		z(other.z)
	{

	}



	FString ToString()
	{
		return FStringFormatter::Format("{ X:%0 Y:%1 Z:%2 }", x, y, z);
	}

	GenType Length() const
	{
		return sqrt(LengthSquared());
	}

	GenType LengthSquared() const
	{
		return x * x + y * y + z * z;
	}

	static TVector3 Lerp(const TVector3& l, const TVector3& r, GenType t)
	{
		return l + (r - l) * t;
	}

	static TVector3 Cross(const TVector3& l, const TVector3& r)
	{
		return TVector3(
			l.y * r.z - l.z * r.y,
			l.z * r.x - l.x * r.z,
			l.x * r.y - l.y * r.x
		);
	}

	static GenType Dot(const TVector3& l, const TVector3& r)
	{
		return l.x * r.x + l.y * r.y + l.z * r.z;
	}

	static TVector3 Normalize(const TVector3& v)
	{
		return v / v.Length();
	}
	static TVector3 Max(const TVector3& a, const TVector3& b)
	{
		return TVector3(
			max(a.x, b.x),
			max(a.y, b.y),
			max(a.z, b.z)
		);
	}

	static TVector3 Min(const TVector3& a, const TVector3& b)
	{
		return TVector3(
			min(a.x, b.x),
			min(a.y, b.y),
			min(a.z, b.z)
		);
	}



	GenType& operator[](size_t _index)
	{
		assert(_index < 3);

		return data[_index];
	}
	const GenType& operator[](size_t _index) const
	{
		assert(_index < 3);

		return data[_index];
	}

	TVector3& operator-()
	{
		this->x = -this->x;
		this->y = -this->y;
		this->z = -this->z;

		return *this;
	}


	TVector3& operator=(const TVector3& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;

		return *this;
	}

	TVector3& operator+=(const TVector3& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;

		return *this;
	}
	TVector3& operator-=(const TVector3& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;

		return *this;
	}

	TVector3& operator*=(const GenType& s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;

		return *this;
	}

	TVector3& operator/=(const GenType& s)
	{
		this->x /= s;
		this->y /= s;
		this->z /= s;

		return *this;
	}


	bool IsAlmostZero() const
	{
		return 
			x < FLOAT_EPSILON && 
			y < FLOAT_EPSILON && 
			z < FLOAT_EPSILON;
	}


	// Predefined

};

#include "TVector3.inl"


