#pragma once


#include "Core/Common/CoreCommon.h"



EXPORT(class, FVector2)
{
public:
	union
	{
		struct { float x, y; };
		struct { float r, g; };
		struct { float s, t; };

		struct { float data[2]; };
	};

	constexpr FVector2()						: x(0), y(0) {}
	FVector2(float _v)				: x(_v), y(_v) {}
	FVector2(float _x, float _y)		: x(_x), y(_y) {}

	FString ToString();

	float Length();
	float LengthSquared();


	static const FVector2& Lerp(const FVector2& l, const FVector2& r, const float& t);
	static const float& Dot(const FVector2& l, const FVector2& r);
	static FVector2 Normalize(FVector2 v);


	float& operator[](size_t _index)
	{
		assert(_index < 2);

		return data[_index];
	}

	FVector2& operator=(const FVector2& v)
	{
		this->x = v.x;
		this->y = v.y;

		return *this;
	}


};

FLAME_API bool					operator==(const FVector2& l, const FVector2& r);
FLAME_API bool					operator!=(const FVector2& l, const FVector2& r);

FLAME_API FVector2			operator*(const FVector2& l, const FVector2& r);
FLAME_API FVector2			operator/(const FVector2& l, const FVector2& r);
FLAME_API FVector2			operator*(const FVector2& l, const float& _scalar);
FLAME_API FVector2			operator/(const FVector2& l, const float& _scalar);
FLAME_API FVector2			operator+(const FVector2& l, const FVector2& r);
FLAME_API FVector2			operator-(const FVector2& l, const FVector2& r);