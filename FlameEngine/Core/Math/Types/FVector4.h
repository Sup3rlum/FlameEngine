#pragma once


#include "Core/Common/CoreCommon.h"
#include "FVector2.h"
#include "FVector3.h"
#include "../SIMD/simddef.h"


EXPORT(class, FVector4)
{
public:

	union
	{
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
		struct { float s, t, p, q; };

		struct { float x;  FVector2 yz; float w; };
		struct { float r;  FVector2 gb; float a; };
		struct { float s;  FVector2 tp; float q; };

		struct { FVector2 xy; FVector2 zw; };
		struct { FVector2 rg; FVector2 ba; };
		struct { FVector2 st; FVector2 pq; };

		struct { FVector3 xyz; float w; };
		struct { FVector3 rgb; float a; };
		struct { FVector3 stp; float q;  };

		struct { float x; FVector3 yzw; };
		struct { float r; FVector3 gba; };
		struct { float s; FVector3 tpq; };

		struct { float data[4]; };
		struct { __m128 mmv; };

;
	};

	constexpr FVector4() : x(0), y(0), z(0), w(0) {}

	FVector4(float _v) : x(_v), y(_v), z(_v), w(_v) {}

	FVector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

	FVector4(FVector3 _vec, float _w) : x(_vec.x), y(_vec.y), z(_vec.z), w(_w) {}

	FVector4(FVector2 _vec1, FVector2 _vec2) : x(_vec1.x), y(_vec1.y), z(_vec2.x), w(_vec2.y) {}

	FVector4(__m128 pv) : mmv(pv) {}



	FString ToString();


	float Length(); 
	float LengthSquared();


	static const float& Dot(const FVector4& l, const FVector4& r);
	static FVector4 Normalize(const FVector4& v);



	float& operator[](size_t _index)
	{
		assert(_index < 4);

		return data[_index];
	}

	const float& operator[](size_t _index) const
	{
		assert(_index < 4);

		return data[_index];
	}


	FVector4& operator=(const FVector4& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		this->w = v.w;

		return *this;
	}


	FVector4& operator+=(const FVector4& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		this->w += v.w;

		return *this;
	}
	FVector4& operator-=(const FVector4& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		this->w -= v.w;

		return *this;
	}

	FVector4& operator*=(const float& s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;
		this->w *= s;

		return *this;
	}
	FVector4& operator/=(const float& s)
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



FLAME_API bool					operator==(const FVector4& l, const FVector4& r);
FLAME_API bool					operator!=(const FVector4& l, const FVector4& r);

FLAME_API FVector4			operator*(const FVector4& l, const FVector4& r);
FLAME_API FVector4			operator/(const FVector4& l, const FVector4& r);
FLAME_API FVector4			operator*(const FVector4& l, const float& _scalar);
FLAME_API FVector4			operator/(const FVector4& l, const float& _scalar);
FLAME_API FVector4			operator+(const FVector4& l, const FVector4& r);
FLAME_API FVector4			operator-(const FVector4& l, const FVector4& r);