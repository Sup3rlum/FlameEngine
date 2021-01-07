#pragma once


#include "Core/Common/CoreCommon.h"
#include "FVector2.h"


EXPORT(class, FVector3) 
{
public:
	
	union
	{
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float s, t, p; };
		struct { FVector2 xy; float z; };
		struct { FVector2 rg; float b; };
		struct { FVector2 st; float p; };
		struct { float x;  FVector2 yz; };
		struct { float r;  FVector2 gb; };
		struct { float s;  FVector2 tp; };
		struct { float data[3]; };
	};


	constexpr FVector3() : x(0), y(0), z(0) {}

	FVector3(float _v) : x(_v), y(_v), z(_v) {}
	FVector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	FVector3(FVector2 _vec, float _z) : x(_vec.x), y(_vec.y), z(_z) {}



	FString ToString();

	float Length();
	constexpr float LengthSquared();

	static FVector3 Lerp(const FVector3& l, const FVector3& r, const float& t);

	static FVector3 Cross(const FVector3& l, const FVector3& r);
	static const float& Dot(const FVector3& l, const FVector3& r);

	static FVector3 Normalize(const FVector3& v);

	static FVector3 Max(const FVector3& a, const FVector3& b);
	static FVector3 Min(const FVector3& a, const FVector3& b);

	float& operator[](size_t _index)
	{
		assert(_index < 3);

		return data[_index];
	}
	const float& operator[](size_t _index) const
	{
		assert(_index < 3);

		return data[_index];
	}

	FVector3& operator=(const FVector3& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;

		return *this;
	}

	FVector3& operator+=(const FVector3& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;

		return *this;
	}
	FVector3& operator-=(const FVector3& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;

		return *this;
	}

	FVector3& operator*=(const float& s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;

		return *this;
	}

	FVector3& operator/=(const float& s)
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

};


FLAME_API FVector3			operator-(const FVector3& l);

FLAME_API bool					operator==(const FVector3& l, const FVector3& r);
FLAME_API bool					operator!=(const FVector3& l, const FVector3& r);

FLAME_API FVector3			operator*(const FVector3& l, const FVector3& r);
FLAME_API FVector3			operator/(const FVector3& l, const FVector3& r);
FLAME_API FVector3			operator*(const FVector3& l, const float& _scalar);
FLAME_API FVector3			operator/(const FVector3& l, const float& _scalar);
FLAME_API FVector3			operator+(const FVector3& l, const FVector3& r);
FLAME_API FVector3			operator-(const FVector3& l, const FVector3& r);
FLAME_API FVector3			operator^(const FVector3& l, const FVector3& r);
FLAME_API float			operator&(const FVector3& l, const FVector3& r);