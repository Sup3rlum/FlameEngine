#pragma once

#include "../dll/framework.h"
#include "../util/macros.h"

EXPORT_STRUCT FLAME_GPU
{
public:
	STRING oemVendor;
	STRING oemString;
	STRING driverString;

	ULONGLONG vMemory;
};