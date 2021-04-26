#pragma once

#include "CoreBase.h"

template <typename T>
FORCEINLINE constexpr T Align(T Val, uint64 Alignment)
{
	return (T)(((uint64)Val + Alignment - 1) & ~(Alignment - 1));
}

template <typename T>
FORCEINLINE constexpr T AlignDown(T Val, uint64 Alignment)
{
	return (T)(((uint64)Val) & ~(Alignment - 1));
}

template <typename T>
FORCEINLINE constexpr T AlignNonPowerOf2(T Val, uint64 Alignment)
{

	return (T)((((uint64)Val + Alignment - 1) / Alignment) * Alignment);
}

template <typename T>
FORCEINLINE constexpr T AlignDownNonPowerOf2(T Val, uint64 Alignment)
{

	return (T)((((uint64)Val) / Alignment) * Alignment);
}
