#pragma once

#include "nchfx.h"

class DELUSION_DLL FrameTime
{
public:

	FrameTime();

	double DeltaTime;
	double TotalTime;
	DELuint64 EllapsedFrames;
};

