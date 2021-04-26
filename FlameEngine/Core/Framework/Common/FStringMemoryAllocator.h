#pragma once


#include "FStringConversionTraits.h"



struct FBaseStringMemoryAllocator
{
	FORCEINLINE static void Allocate(FChar* Dst, const char* Src, size_t elemCount)
	{
		FChar* buffer = Memory::AllocCounted<FChar>(elemCount + 1);

		size_t outSize;
		mbstowcs_s(&outSize, buffer, elemCount + 1, Src, elemCount);

		memcpy(Dst, buffer, elemCount * sizeof(FChar));

		free(buffer);
	}
	FORCEINLINE static void Allocate(FChar* Dst, const FChar* Src, size_t elemCount)
	{
		memcpy(Dst, Src, elemCount * sizeof(FChar));
	}
	FORCEINLINE static void Allocate(char* Dst, const char* Src, size_t elemCount)
	{
		memcpy(Dst, Src, elemCount * sizeof(char));
	}




	// WCHAR
	template<typename BasicType>
	FORCEINLINE static void Allocate(FChar* Dst, BasicType d, size_t elemCount)
	{
		_snwprintf_s(Dst, elemCount + 1, elemCount, FBaseStringPlatformTypeConversionSpecifier<FChar>::GetSpecifier<BasicType>(), d);
	}
	// CHAR
	template<typename BasicType>
	FORCEINLINE static void Allocate(char* Dst, BasicType d, size_t elemCount)
	{
		snprintf(Dst, elemCount + 1, FBaseStringPlatformTypeConversionSpecifier<char>::GetSpecifier<BasicType>(), d);
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