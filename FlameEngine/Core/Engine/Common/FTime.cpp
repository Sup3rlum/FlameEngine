#include "FTime.h"
#include <profileapi.h>

FTimeStamp FTimeStamp::MarkCurrent()
{
	LARGE_INTEGER t;
	FTimeStamp timestamp;

	QueryPerformanceCounter(&t);
	timestamp.data = t.QuadPart;

	return timestamp;
}


FTimeType FTime::PlatformTickFrequency()
{
	LARGE_INTEGER t;
	t.QuadPart = 0;
	QueryPerformanceFrequency(&t);
	return t.QuadPart;
}