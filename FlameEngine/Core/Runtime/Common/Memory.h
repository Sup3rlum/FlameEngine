#pragma once

#include "Core/Common/CoreBase.h"

#include "Core/Common/CoreFwdDeclarations.h"




EXPORT(class,  Memory)
{
public:

	template <typename T>
	FORCEINLINE static T* Alloc(size_t byteSize)
	{
		return (T*)malloc(byteSize);
	}

	template <typename T>
	FORCEINLINE static T* AllocCounted(size_t count)
	{
		return Alloc<T>(sizeof(T) * count);
	}



	template <typename T>
	FORCEINLINE static void Zero(T* mem, size_t byteSize)
	{
		memset(mem, 0, byteSize);
	}


	template <typename T>
	FORCEINLINE static void Free(T* mem)
	{
		free(mem);
	}



	template <typename T>
	FORCEINLINE static void Copy(T* dst, T* source, size_t byteSize)
	{
		memcpy(dst, source, byteSize);
	}
	template <typename T>
	FORCEINLINE static void Copy(T* dst, const T* source, size_t byteSize)
	{
		memcpy(dst, source, byteSize);
	}
	template <typename T>
	FORCEINLINE static void CopyCounted(T* dst, T* source, size_t elemCount)
	{
		memcpy(dst, source, sizeof(T) * elemCount);
	}
	template <typename T>
	FORCEINLINE static void CopyCounted(T* dst, const T* source, size_t elemCount)
	{
		memcpy(dst, source, sizeof(T) * elemCount);
	}

	template <typename TDst, typename TSource>
	FORCEINLINE static void CopyReinterpret(TDst* dst, TSource* source, size_t size)
	{
		memcpy(dst, source, size);
	}

	template <typename T>
	FORCEINLINE static void CopyOverlap(T* dst, T* source, size_t size)
	{
		memmove(dst, source, size);
	}


	template <typename TData>
	FORCEINLINE static TData* Realloc(TData* Src, size_t Size)
	{
		TData* newPtr = Alloc(Size);
		Copy(newPtr, Src, Size);
		Free(Src);
		return newPtr;
	}

};

