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
	};


	constexpr FVector3() : x(0), y(0), z(0) {}

	constexpr FVector3(float _v) : x(_v), y(_v), z(_v) {}
	constexpr FVector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	constexpr FVector3(FVector2 _vec, float _z) : x(_vec.x), y(_vec.y), z(_z) {}



	FString ToString();

	float Length();
	constexpr float LengthSquared();

	static FVector3 Lerp(FVector3 const& l, FVector3 const& r, float const& t);

	static FVector3 Cross(FVector3 const& l, FVector3 const& r);
	static float const& Dot(FVector3 const& l, FVector3 const& r);

	static FVector3 Normalize(FVector3 v);

	static FVector3 Max(FVector3 a, FVector3 b);
	static FVector3 Min(FVector3 a, FVector3 b);

	constexpr float& operator[](size_t _index)
	{
		assert(_index < 3);

		if (_index == 0)
			return x;
		else if (_index == 1)
			return y;
		else
			return z;
	}

	constexpr FVector3& operator=(FVector3 const& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;

		return *this;
	}

	constexpr FVector3& operator+=(FVector3 const& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;

		return *this;
	}
	constexpr FVector3& operator-=(FVector3 const& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;

		return *this;
	}

	constexpr FVector3& operator*=(float const& s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;

		return *this;
	}

	constexpr FVector3& operator/=(float const& s)
	{
		this->x /= s;
		this->y /= s;
		this->z /= s;

		return *this;
	}

};


FLAME_API FVector3			operator-(FVector3 const& l);

FLAME_API bool					operator==(FVector3 const& l, FVector3 const& r);
FLAME_API bool					operator!=(FVector3 const& l, FVector3 const& r);

FLAME_API FVector3			operator*(FVector3 const& l, FVector3 const& r);
FLAME_API FVector3			operator/(FVector3 const& l, FVector3 const& r);
FLAME_API FVector3			operator*(FVector3 const& l, float const& _scalar);
FLAME_API FVector3			operator/(FVector3 const& l, float const& _scalar);
FLAME_API FVector3			operator+(FVector3 const& l, FVector3 const& r);
FLAME_API FVector3			operator-(FVector3 const& l, FVector3 const& r);
FLAME_API FVector3			operator^(FVector3 const& l, FVector3 const& r);
FLAME_API float			operator&(FVector3 const& l, FVector3 const& r);