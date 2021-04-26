#pragma once

#include "Core/Common/CoreCommon.h"

typedef long long FTimeType;


EXPORT(struct, FTimeContainer)
{
public:

	FTimeType data;

	bool operator<(FTimeContainer & other)
	{
		return data < other.data;
	}
	bool operator>(FTimeContainer& other)
	{
		return data > other.data;
	}
};



EXPORT(struct, FTimeStamp) : public FTimeContainer
{
public:

	static FTimeStamp MarkCurrent();
};


EXPORT(class, FTime)
{
public:
	static FTimeType PlatformTickFrequency();


};

