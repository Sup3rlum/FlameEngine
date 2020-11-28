#pragma once

#include "../dll/framework.h"
#include "../util/macros.h"

EXPORT_STRUCT FLAME_CPU
{
public:
	STRING oemString;
	

	INT numSockets;
	INT numCores;
	INT numThreads;
	INT baseClock;
	bool hasVirtualization;
	bool SupportsAVX;
	bool SupportsAVX2;
	bool SupportsSSE4;
};