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

float FTimeSpan::GetSeconds()
{
	return (float)Data / (float)FTime::PlatformTickFrequency().Data;
}

float FTimeSpan::GetMilliseconds()
{
	return GetSeconds() * 1000.0f;
}

float FTimeSpan::GetMinutes()
{
	return GetSeconds() / 60.0f;
}
float FTimeSpan::GetHours()
{
	return GetSeconds() / 3600.0f;
}