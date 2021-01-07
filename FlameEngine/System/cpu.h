#pragma once

#include "Core/Common/CoreCommon.h"

EXPORT(struct,  FLAME_CPU)
{
public:
	FString oemString;
	

	INT numSockets;
	INT numCores;
	INT numThreads;
	INT baseClock;
	bool hasVirtualization;
	bool SupportsAVX;
	bool SupportsAVX2;
	bool SupportsSSE4;
};