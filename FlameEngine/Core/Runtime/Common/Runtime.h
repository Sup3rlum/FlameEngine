#pragma once


#include "Core/Common/CoreBase.h"
#include "System/systeminfo.h"

EXPORT(class,  Runtime)
{
public:

	SystemInfo sysInfo;

	static Runtime* handlingInstance;
};

