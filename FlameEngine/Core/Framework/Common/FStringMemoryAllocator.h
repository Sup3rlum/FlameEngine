#pragma once


#include "FStringConversionTraits.h"



struct FBaseStringMemoryAllocator
{
	FORCEINLINE static void Allocate(FChar* Dst, const char* Src, size_t elemCount)
	{
		FChar* buffer = Memory::AllocCounted<FChar>(elemCount + 1);

		size_t outSize;
		mbstowcs_s(&outSize, buffer, elemCount + 1, Src, elemCount);

		Memory::CopyCounted(Dst, buffer, elemCount);
		Memory::Free(buffer);
	}
	FORCEINLINE static void Allocate(FChar* Dst, const FChar* Src, size_t elemCount)
	{
		Memory::CopyCounted(Dst, Src, elemCount);
	}
	FORCEINLINE static void Allocate(char* Dst, const char* Src, size_t elemCount)
	{
		Memory::CopyCounted(Dst, Src, elemCount);
	}



	FORCEINLINE static void Allocate(FChar* Dst, FChar d, size_t elemCount)
	{
		Dst[0] = d;
	}
	FORCEINLINE static void Allocate(char* Dst, char d, size_t elemCount)
	{
		Dst[0] = d;
	}


	FORCEINLINE static void Allocate(FChar* Dst, char d, size_t elemCount)
	{
		Dst[0] = (FChar)d;
	}
	FORCEINLINE static void Allocate(char* Dst, FChar d, size_t elemCount)
	{
		Dst[0] = (char)d;
	}


	// WCHAR
	template<typename BasicType>
	FORCEINLINE static void Allocate(FChar* Dst, BasicType d, size_t elemCount)
	{
		FChar* buffer = Memory::AllocCounted<FChar>(elemCount + 1);
		_snwprintf_s(buffer, elemCount + 1, elemCount, FBaseStringPlatformTypeConversionSpecifier<FChar>::GetSpecifier<BasicType>(), d);
		Memory::CopyCounted(Dst, buffer, elemCount);
		Memory::Free(buffer);
	}
	// CHAR
	template<typename BasicType>
	FORCEINLINE static void Allocate(char* Dst, BasicType d, size_t elemCount)
	{
		char* buffer = Memory::AllocCounted<char>(elemCount + 1);
		snprintf(buffer, elemCount + 1, FBaseStringPlatformTypeConversionSpecifier<char>::GetSpecifier<BasicType>(), d);
		Memory::CopyCounted(Dst, buffer, elemCount);
		Memory::Free(buffer);
	}



	FORCEINLINE static size_t GetLength(const char* cstr)
	{
		return strlen(cstr);
	}
	FORCEINLINE static size_t GetLength(const FChar* cstr)
	{
		return wcslen(cstr);
	}

	FORCEINLINE static size_t GetLength(const char)
	{
		return 1;
	}
	FORCEINLINE static size_t GetLength(const FChar)
	{
		return 1;
	}


	template<typename BasicType>
	FORCEINLINE static size_t GetLength(BasicType param)
	{
		return snprintf(NULL, 0, FBaseStringPlatformTypeConversionSpecifier<char>::GetSpecifier<BasicType>(), param);
	}

};