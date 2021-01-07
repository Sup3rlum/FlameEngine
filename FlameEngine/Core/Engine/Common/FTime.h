#pragma once

#include "Core/Common/CoreCommon.h"

typedef long long FTimeType;


EXPORT(struct, FTimeContainer)
{
protected:

	FTimeType data;

public:
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
	static FTimeType PlatformTickFrequency();


};

