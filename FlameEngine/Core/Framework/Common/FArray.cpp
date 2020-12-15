#include "FArray.h"


template<typename GenType>
FArray<GenType>::FArray()
{
	size = 0;
	capacity = 0;
	ptrInternal = NULL;
}

template<typename GenType>
FArray<GenType>::FArray(const FArray<GenType>& arr)
{
	size = arr.size;
	capacity = arr.capacity;
	ptrInternal = Memory::Create<GenType>(size);

	memcpy(ptrInternal, arr.ptrInternal, size);
}

template<typename GenType>
FArray<GenType>::FArray(size_t initSize)
{
	size = initSize;
	capacity = initSize;
	ptrInternal = Memory::Create<GenType>(initSize);

	Memory::Zero(ptrInternal);

}




template<typename GenType>
FArray<GenType>& FArray<GenType>::operator=(const FArray<GenType>& arr)
{
	delete[] ptrInternal;

	size = arr.size;
	capacity = arr.capacity;
	ptrInternal = Memory::Create<GenType>(size);

	memcpy(ptrInternal, arr.ptrInternal, size);

	return *this;
}


template<typename GenType>
void FArray<GenType>::Reserve(size_t newcapacity)
{
	if (ptrInternal == NULL)
	{
		size = 0;
		capacity = 0;
	}

	GenType* Newbuffer = Memory::Create<GenType>(newcapacity);

	unsigned int l_Size = min(newcapacity, size);


	memcpy(NewBuffer, ptrInternal, l_Size)

	capacity = newcapacity;
	delete[] ptrInternal;
	ptrInternal = Newbuffer;
}

template<typename GenType>
void FArray<GenType>::Resize(size_t newsize)
{
	Reserve(newsize);
	size = newsize;
}


template<typename GenType>
bool FArray<GenType>::IsEmpty()
{
	return ptrInternal == NULL || size == 0 || capacity == 0;
}




template<typename GenType>
GenType& FArray<GenType>::First()
{
	return ptrInternal[0];
}

template<typename GenType>
GenType& FArray<GenType>::Last()
{
	return ptrInternal[size - 1];
}

template<typename GenType>
GenType& FArray<GenType>::operator[](size_t index)
{
	return ptrInternal[index];
}


template<typename GenType>
size_t FArray<GenType>::Size()
{
	return size;
}

template<typename GenType>
void FArray<GenType>::Clear()
{

	//Memory::Zero(ptrInternal, size);

	capacity = 0;
	size = 0;
	ptrInternal = NULL;
}
