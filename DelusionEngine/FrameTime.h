#pragma once

#include "nchfx.h"

EXPORT_CLASS FrameTime
{
public:

	FrameTime();

	double DeltaTime;
	double TotalTime;
	DELuint64 EllapsedFrames;
};

