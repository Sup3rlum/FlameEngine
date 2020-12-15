#pragma once

#include "Core/Common/CoreBase.h"

EXPORT_FORWARD_DECL(class, FString);


EXPORT(class, FObject)
{
public:
	virtual FString ToString();
};
