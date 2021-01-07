#pragma once

#include "Core/Common/CoreBase.h"

#include "Core/Common/CoreFwdDeclarations.h"

EXPORT(class,  Memory)
{
public:

	template <typename T>
	static T* Create(size_t count)
	{
		T* mem = (T*)malloc(sizeof(T) * count);
		return mem;
	}

	template <typename T>
	static void Zero(T* mem, size_t count)
	{
		memset(mem, 0, count);
	}


	template <typename T>
	static void Free(T* mem)
	{
		free(mem);
	}



	template <typename T>
	static void Copy(T* dst, T* source, size_t size)
	{
		memcpy(dst, source, size);
	}

	template <typename TDst, typename TSource>
	static void CopyReinterpret(TDst* dst, TSource* source, size_t size)
	{
		memcpy(dst, source, size);
	}

	template <typename T>
	static void CopyOverlap(T* dst, T* source, size_t size)
	{
		memmove(dst, source, size);
	}


	template <typename TData>
	static TData* Realloc(TData* Src, size_t Size)
	{
		TData* newPtr = Create(Size);
		Copy(newPtr, Src, size);
		Free(Src);
		return newPtr;
	}

};

