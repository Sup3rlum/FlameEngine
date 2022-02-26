#pragma once

#include "Core/Common/CoreCommon.h"


EXPORT(struct, FTimeSpan)
{
private:
	union
	{
		int64 Data;
		LARGE_INTEGER llWinData;
	};

public:
	FTimeSpan() : FTimeSpan(0)
	{

	}
	FTimeSpan(int64 data) :
		Data(data)
	{

	}

	int64 GetPlatformTicks();
	float GetSeconds();
	float GetMilliseconds();
	float GetHours();
	float GetMinutes();


	friend FTimeSpan operator-(const FTimeSpan& t1, const FTimeSpan& t2)
	{
		return FTimeSpan(t1.Data - t2.Data);
	}
	friend FTimeSpan operator+(const FTimeSpan& t1, const FTimeSpan& t2)
	{
		return FTimeSpan(t1.Data + t2.Data);
	}

	FTimeSpan& operator+=(const FTimeSpan& t)
	{
		this->Data += t.Data;

		return *this;
	}
	FTimeSpan& operator-=(const FTimeSpan& t)
	{
		this->Data -= t.Data;

		return *this;
	}

	friend class FTime;
};

EXPORT(class, FTime)
{
public:
	static FTimeSpan PlatformTickFrequency();
	static FTimeSpan GetTimestamp();


};

struct FGameTime
{
	FTimeSpan DeltaTime;
	FTimeSpan TotalTime;
	int64 TotalTicks = 0;

};