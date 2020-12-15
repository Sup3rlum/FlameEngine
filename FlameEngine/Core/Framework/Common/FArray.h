#pragma once


#include "Core/Common/CoreBase.h"
#include "Core/Runtime/Common/Memory.h"

template<typename GenType>
EXPORT(struct, FArray)
{
public:

	typedef T* IteratorType;

	FArray();
	FArray(const FArray<GenType> &);
	FArray(size_t);

	size_t Size();
	size_t AllocatorSize();


	void Resize(size_t size);

	bool IsEmpty();
	void Clear();

	IteratorType Begin();
	IteratorType End();

	GenType& First();
	GenType& Last();

	GenType& operator[](size_t index);
	FArray<GenType>& operator=(const FArray<GenType> &);

	void Add(GenType);

	void Resize(size_t size);
	void Reserve(size_t capacity);

protected:
	size_t size;
	size_t capacity;
	GenType* ptrInternal;

};

