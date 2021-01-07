#pragma once

#include "Core/Common/CoreCommon.h"


EXPORT(struct, FEngineVersion)
{
public:
	uint32 Major;
	uint32 Minor;
	uint32 Stepping;
	uint32 Version;
	FStaticString<64> Attribute;
};


FORCEINLINE bool operator==(const FEngineVersion& a, const FEngineVersion& b)
{
	return !memcmp(&a, &b, 4 * sizeof(uint32)) && a.Attribute == b.Attribute;
}

FORCEINLINE bool operator!=(const FEngineVersion& a, const FEngineVersion& b)
{
	return !(a == b);
}