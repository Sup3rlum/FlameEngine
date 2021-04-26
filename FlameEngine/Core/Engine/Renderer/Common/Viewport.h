#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"


struct FScreenRectangle
{
	union
	{
		struct { float X, Y; FVector2 Size; };
		struct { FVector2 Origin; float Width, Height;  };
	};

	FScreenRectangle(float x, float y, float width, float height) : X(x), Y(y), Size(FVector2(width, height))
	{}

};