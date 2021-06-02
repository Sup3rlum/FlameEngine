#pragma once


#include "Core/Common/CoreCommon.h"
#include "TVector3.h"
#include "../SIMD/simddef.h"

template<typename GenType>
class TVector4
{
public:

	union
	{
		struct { GenType x, y, z, w; };
		struct { GenType r, g, b, a; };
		struct { GenType s, t, p, q; };

		struct { GenType x;  TVector2<GenType> yz; GenType w; };
		struct { GenType r;  TVector2<GenType> gb; GenType a; };
		struct { GenType s;  TVector2<GenType> tp; GenType q; };

		struct { TVector2<GenType> xy; TVector2<GenType> zw; };
		struct { TVector2<GenType> rg; TVector2<GenType> ba; };
		struct { TVector2<GenType> st; TVector2<GenType> pq; };

		struct { TVector3<GenType> xyz; GenType w; };
		struct { TVector3<GenType> rgb; GenType a; };
		struct { TVector3<GenType> stp; GenType q;  };

		struct { GenType x; TVector3<GenType> yzw; };
		struct { GenType r; TVector3<GenType> gba; };
		struct { GenType s; TVector3<GenType> tpq; };

		struct { GenType data[4]; };
		struct { __m128 mmv; };

;
	};

	 TVector4() : 
		 x(0), 
		 y(0), 
		 z(0), 
		 w(0) {}

	TVector4(GenType v) : 
		x(v), 
		y(v), 
		z(v), 
		w(v) {}

	TVector4(GenType x, GenType y, GenType z, GenType w) : 
		x(x), 
		y(y), 
		z(z), 
		w(w) {}

	TVector4(TVector3<GenType> vec, GenType w) :
		x(vec.x), 
		y(vec.y), 
		z(vec.z), 
		w(w) {}

	TVector4(TVector2<GenType> vec1, TVector2<GenType> vec2) :
		x(vec1.x), 
		y(vec1.y), 
		z(vec2.x), 
		w(vec2.y) {}

	TVector4(__m128 pv) : mmv(pv) {}


	template<typename OtherType>
	TVector4(const TVector4<OtherType> other) :
		x(other.x),
		y(other.y),
		z(other.z),
		w(other.w)
	{

	}



	FString ToString()
	{
		return FString::Format("{ X:%0 Y:%1 Z:%2 W:%3 }", x, y, z, w);
	}



	GenType Length() const
	{
		return sqrt(LengthSquared());
	}


	GenType LengthSquared() const
	{
		return x * x + y * y + z * z + w * w;
	}


	static GenType Dot(const TVector4& l, const TVector4& r)
	{
		return l.x * r.x + l.y * r.y + l.z * r.z + l.w * r.w;
	}


	static TVector4 Normalize(const TVector4& v)
	{
		return v / v.Length();
	}


	static TVector4 Lerp(const TVector4& l, const TVector4& r, GenType t)
	{
		return l + (r - l) * t;
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


	TVector4& operator-()
	{
		this->x = -this->x;
		this->y = -this->y;
		this->z = -this->z;
		this->w = -this->w;

		return *this;
	}



	TVector4& operator=(const TVector4& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		this->w = v.w;

		return *this;
	}


	TVector4& operator+=(const TVector4& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		this->w += v.w;

		return *this;
	}
	TVector4& operator-=(const TVector4& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		this->w -= v.w;

		return *this;
	}

	TVector4& operator*=(const GenType& s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;
		this->w *= s;

		return *this;
	}
	TVector4& operator/=(const GenType& s)
	{
		this->x /= s;
		this->y /= s;
		this->z /= s;
		this->w /= s;

		return *this;
	}
	operator __m128()
	{
		return mmv;
	}

};




#include  "TVector4.inl"