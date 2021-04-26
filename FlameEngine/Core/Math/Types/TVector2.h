#pragma once


#include "../MathBase.h"
#include "../Constants.h"

template<typename GenType>
class TVector2
{
public:
	union
	{
		struct { GenType x, y; };
		struct { GenType r, g; };
		struct { GenType s, t; };

		struct { GenType data[2]; };
	};

	TVector2() : 
		x(0), 
		y(0) {}

	TVector2(GenType v) : 
		x(v), 
		y(v) {}

	TVector2(GenType x, GenType y) : 
		x(x), 
		y(y) {}


	template<typename OtherType>
	TVector2(const TVector2<OtherType> other) :
		x(other.x),
		y(other.y)
	{

	}

	FString ToString()
	{
		return FString::Format("{ X:%0 Y:%1 }", x, y);
	}

	GenType Length() const
	{
		return sqrt(LengthSquared());
	}

	GenType LengthSquared() const
	{
		return x * x + y * y;
	}


	static TVector2 Lerp(const TVector2& l, const TVector2& r, const GenType& t)
	{
		return l + (r - l) * t;
	}

	static GenType Dot(const TVector2& l, const TVector2& r)
	{
		return l.x * r.x + l.y * r.y;
	}
	static TVector2 Normalize(TVector2 v)
	{
		return v / v.Length();
	}


	GenType& operator[](size_t _index)
	{
		assert(_index < 2);

		return data[_index];
	}
	const GenType& operator[](size_t _index) const
	{
		assert(_index < 2);

		return data[_index];
	}

	TVector2& operator-()
	{
		this->x = -this->x;
		this->y = -this->x;

		return *this;
	}


	TVector2& operator=(const TVector2& v)
	{
		this->x = v.x;
		this->y = v.y;

		return *this;
	}


};

#include "TVector2.inl"