#pragma once


#include "cpu.h"
#include "gpu.h"
#include "os.h"
#include "system_memory.h"

EXPORT(struct,  SystemInfo)
{
public:
	FLAME_CPU cpuInfo;
	FLAME_GPU gpuInfo;
	FLAME_OS osInfo;
	FLAME_SYSTEM_MEMORY memoryInfo;
};


