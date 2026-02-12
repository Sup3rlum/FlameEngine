#include "FTime.h"
#include <profileapi.h>

FTimeSpan FTime::GetTimestamp()
{
	FTimeSpan timestamp;
	QueryPerformanceCounter(&timestamp.llWinData);
	return timestamp;
}


FTimeSpan FTime::PlatformTickFrequency()
{
	FTimeSpan tickFrequency;
	QueryPerformanceFrequency(&tickFrequency.llWinData);
	return tickFrequency;
}

int64 FTimeSpan::GetPlatformTicks()
{
	return Data;
}

double FTimeSpan::GetSeconds()
{
	return (double)Data / (double)(FTime::PlatformTickFrequency().Data);
}

double FTimeSpan::GetMilliseconds()
{
	return GetSeconds() * 1000.0f;
}

double FTimeSpan::GetMinutes()
{
	return GetSeconds() / 60.0f;
}
double FTimeSpan::GetHours()
{
	return GetSeconds() / 3600.0f;
}