#pragma once

#include "Core/Common/CoreBase.h"

#include "Core/Common/CoreFwdDeclarations.h"

EXPORT(class,  Memory)
{
public:
	static FString	ToString(FArray<BYTE> bytedata);

	template<typename T>
	static T ToType(FArray<T> bytedata)
	{
		T t;
		memcpy(&t, data, sizeof(T));
		return t;
	}

	template<typename T>
	static FArray<T> GetByteMemory(T)
	{
		FArray<T> mem;

		return mem;
	}



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
	void Free(T* mem)
	{
		delete[] mem;
	}
};

