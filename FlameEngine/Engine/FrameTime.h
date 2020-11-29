#pragma once

#include "../dll/nchfx.h"


using namespace std;

typedef chrono::time_point<chrono::steady_clock> TimeSpan;
typedef chrono::duration<double> TimeDuration;

EXPORT(class,  FrameTime)
{
public:


	static void Start();

	static TimeSpan Now();

	static TimeDuration FrameDeltaTime;

	static TimeSpan StartTime;
	static TimeSpan TotalTime;

	static TimeSpan ApplicationRunTime();

	static _UNS_ FL_INT64 EllapsedFrames;
};

