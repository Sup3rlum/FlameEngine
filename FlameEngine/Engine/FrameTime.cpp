#include "FrameTime.h"

TimeDuration FrameTime::FrameDeltaTime = TimeDuration(chrono::nanoseconds(0));
TimeSpan FrameTime::StartTime = TimeSpan(chrono::nanoseconds(0));
TimeSpan FrameTime::TotalTime = TimeSpan(chrono::nanoseconds(0));
DELuint64 FrameTime::EllapsedFrames = 0;

void FrameTime::Start()
{
	StartTime = Now();
}

TimeSpan FrameTime::Now()
{
	return chrono::steady_clock::now();

}
TimeSpan FrameTime::ApplicationRunTime()
{
	return TimeSpan(Now() - StartTime);
}