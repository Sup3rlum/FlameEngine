#pragma once

#include "Core/Runtime/Common/Memory.h"

template<typename GenType>
struct FStack
{
public:

	typedef GenType* IteratorType;

	FStack();
	FStack(const FStack<GenType>&);
	FStack(size_t);



	size_t Size();
	size_t AllocatorSize();

	bool IsEmpty();
	void Clear();



	GenType& First();
	GenType& Last();

	GenType& operator[](size_t index);
	FStack<GenType>& operator=(const FStack<GenType>&);


	void Push(const GenType& element)
	{
		if (size >= capacity)
			Reserve(capacity + 5);
		ptrInternal[size++] = element;
	}

	void Pop()
	{
		if (size > 0)
			size--;
	}


	const GenType& Top()
	{
		return ptrInternal[size - 1];
	}

private:
	void Resize(size_t size);
	void Reserve(size_t capacity);

protected:
	size_t size;
	size_t capacity;
	GenType* ptrInternal;

};



template<typename GenType>
FStack<GenType>::FStack()
{
	size = 0;
	capacity = 0;
	ptrInternal = NULL;
}

template<typename GenType>
FStack<GenType>::FStack(const FStack<GenType>& arr)
{
	size = arr.size;
	capacity = arr.capacity;
	ptrInternal = Memory::Alloc<GenType>(size);

	memcpy(ptrInternal, arr.ptrInternal, size * sizeof(GenType));
}

template<typename GenType>
FStack<GenType>::FStack(size_t initSize)
{
	size = initSize;
	capacity = initSize;
	ptrInternal = Memory::Alloc<GenType>(initSize);

	Memory::Zero(ptrInternal);

}



template<typename GenType>
FStack<GenType>& FStack<GenType>::operator=(const FStack<GenType>& arr)
{
	delete[] ptrInternal;

	size = arr.size;
	capacity = arr.capacity;
	ptrInternal = Memory::Alloc<GenType>(size);

	memcpy(ptrInternal, arr.ptrInternal, size);

	return *this;
}


template<typename GenType>
void FStack<GenType>::Reserve(size_t newcapacity)
{
	if (ptrInternal == NULL)
	{
		size = 0;
		capacity = 0;
	}

	GenType* Newbuffer = Memory::Alloc<GenType>(newcapacity);

	unsigned int l_Size = min(newcapacity, size);


	memcpy(Newbuffer, ptrInternal, l_Size * sizeof(GenType));

	capacity = newcapacity;
	ptrInternal = Newbuffer;
}

template<typename GenType>
void FStack<GenType>::Resize(size_t newsize)
{
	Reserve(newsize);
	size = newsize;
}


template<typename GenType>
bool FStack<GenType>::IsEmpty()
{
	return ptrInternal == NULL || size == 0 || capacity == 0;
}



template<typename GenType>
GenType& FStack<GenType>::operator[](size_t index)
{
	return ptrInternal[index];
}


template<typename GenType>
size_t FStack<GenType>::Size()
{
	return size;
}

template<typename GenType>
void FStack<GenType>::Clear()
{

	//Memory::Zero(ptrInternal, size);

	free(ptrInternal);

	capacity = 0;
	size = 0;
	ptrInternal = NULL;
}


