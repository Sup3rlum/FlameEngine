#pragma once

#include "..\..\dll\nchfx.h"

EXPORT_CLASS RenderableObject
{
public:
	virtual void Render();
	virtual void RenderDepth();
};

