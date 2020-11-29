#pragma once


#include "../../dll/nchfx.h"
#include "../../Mathematics/Module.h"

EXPORT(struct,  UxRectangle)
{
public:
	union
	{
		struct
		{
			fVector2 Position, Size;
		};
		struct
		{
			float x, y, width, height;
		};
	};

	UxRectangle() : Position(0), Size(0) {}
	UxRectangle(fVector2 p, fVector2 s) : Position(p), Size(s) {}


	bool Contains(fVector2 point);
};

