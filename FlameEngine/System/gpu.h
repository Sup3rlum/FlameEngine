#pragma once

#include "Core/Common/CoreCommon.h"

EXPORT(struct,  FLAME_GPU)
{
public:
	FString oemVendor;
	FString oemString;
	FString driverString;

	ULONGLONG vMemory;
};