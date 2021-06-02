#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"


struct FViewportRect
{
	union
	{
		struct { float X, Y; float Width, Height;};
		struct { FVector2 Origin; FVector2 Size;   };
	};

	FViewportRect() :
		Origin(0),
		Size(0)
	{
	}

	FViewportRect(float x, float y, float width, float height) : X(x), Y(y), Width(width), Height(height)
	{}

	FViewportRect(const FViewportRect& other) :
		Origin(other.Origin),
		Size(other.Size)
	{}

	FViewportRect& operator=(const FViewportRect& other)
	{
		Origin = other.Origin;
		Size = other.Size;

		return *this;
	}


	FORCEINLINE float AspectRatio() const
	{
		return Width / Height;
	}

};