#pragma once

#include "Core/Common/CoreCommon.h"


EXPORT(struct, FSerialData)
{

};

EXPORT(struct, ISerializableObject)
{
public:
	virtual FArray<byte> Serialize() = 0;

};