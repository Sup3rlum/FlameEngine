#pragma once

#include "../dll/framework.h"
#include "../util/macros.h"

EXPORT(struct,  FLAME_GPU)
{
public:
	STRING oemVendor;
	STRING oemString;
	STRING driverString;

	ULONGLONG vMemory;
};