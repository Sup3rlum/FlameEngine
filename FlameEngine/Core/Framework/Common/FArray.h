#pragma once


#include "Core/Common/CoreBase.h"
#include "Core/Runtime/Common/Memory.h"





template<typename GenType, size_t GenSize>
EXPORT(class, FStaticArray)
{
public:
	typedef GenType* IteratorType;


	/*
	*	Default Constructor
	*/
	FStaticArray()
	{
	}

	/*
	*	From memory block Constructor
	*/

	FStaticArray(const GenType(&constArr)[GenSize])
	{
		memcpy(dataInternal, constArr, GenSize * sizeof(GenType));
	}
	/*
	*	Copy Constructor
	*/
	FStaticArray(const FStaticArray<GenType, GenSize>&arr)
	{
		memcpy(dataInternal, arr.dataInternal, GenSize * sizeof(GenType));
	}
	/*
	*	C++ why you make me use std :(
	*/
	FStaticArray(std::initializer_list<GenType> list) : FStaticArray(static_cast<const GenType*>(list.begin()), GenSize) 
	{
		assert(GenSize == list.size());
	}


	/* Sizing methods */
	constexpr size_t Length() const
	{
		return GenSize;
	}
	constexpr size_t AllocatorSize() const
	{
		return sizeof(GenType);
	}
	constexpr size_t ByteSize() const
	{
		return GenSize * sizeof(GenType);
	}

	/* State methods */
	bool IsEmpty()
	{
		return dataInternal == NULL || GenSize == 0;
	}


	/* Iteration */
	IteratorType Begin() const
	{
		return dataInternal;
	}
	IteratorType End() const
	{
		return dataInternal + GenSize;
	}

	GenType& First()
	{
		return dataInternal[0];
	}
	const GenType& First() const
	{
		return dataInternal[0];
	}

	GenType& Last()
	{
		return dataInternal[GenSize - 1];
	}

	const GenType& Last() const
	{
		return dataInternal[GenSize - 1];
	}


	/* Operators */

	GenType& operator[](size_t index)
	{

		//static_assert(index < GenSize, "StaticArray Index out of range");

		return dataInternal[index];
	}

	const GenType& operator[](size_t index) const
	{

		//static_assert(index < GenSize, "StaticArray Index out of range");

		return dataInternal[index];
	}

protected:
	GenType dataInternal[GenSize];
};


template<typename GenType>
EXPORT(struct, FArray)
{
public:

	typedef GenType* IteratorType;


	/*
	*	Default Constructor
	*/
	FArray()
	{
		size = 0;
		capacity = 0;
		ptrInternal = NULL;
	}

	FArray(size_t initSize)
	{
		size = initSize;
		capacity = initSize;
		ptrInternal = Memory::Create<GenType>(initSize);

		Memory::Zero(ptrInternal, size);
	}
	/*
	*	From memory block Constructor
	*/
	FArray(const GenType * constArr, size_t _Size)
	{
		size = _Size;
		capacity = _Size;

		ptrInternal = Memory::Create<GenType>(_Size);

		memcpy(ptrInternal, constArr, _Size * sizeof(GenType));

	}
	/*
	*	From FStaticArray constructor
	*/
	template<size_t _Size>
	FArray(const FStaticArray<GenType, _Size>& statarr) : FArray(statarr.Begin(), _Size) {}

	/*
	*	Copy Constructor
	*/
	FArray(const FArray<GenType>&arr) : FArray(arr.ptrInternal, arr.size)
	{
	}
	/*
	*	Move Constructor
	*/
	FArray(FArray<GenType> && arr)
	{
		size = arr.size;
		capacity = arr.capacity;
		ptrInternal = arr.ptrInternal;


		arr.size = 0;
		arr.capacity = 0;
		arr.ptrInternal = NULL;
	}
	/*
	*	C++ why you make me use std :(
	*/
	FArray(std::initializer_list<GenType> list) : FArray(static_cast<const GenType*>(list.begin()), list.size()) {}


	/* Sizing methods */
	size_t Length() const
	{
		return size;
	}
	size_t AllocatorSize() const
	{
		return sizeof(GenType);
	}
	size_t ByteSize() const
	{
		return size * sizeof(GenType);
	}

	/* State methods */
	bool IsEmpty()
	{
		return ptrInternal == NULL || size == 0 || capacity == 0;
	}


	/* Iteration */
	IteratorType Begin() const
	{
		return ptrInternal;
	}
	IteratorType End() const
	{
		return ptrInternal + size;
	}

	GenType& First()
	{
		return ptrInternal[0];
	}
	const GenType& First() const
	{
		return ptrInternal[0];
	}

	GenType& Last()
	{
		return ptrInternal[size - 1];
	}

	const GenType& Last() const
	{
		return ptrInternal[size - 1];
	}



	/* Operators */

	GenType& operator[](size_t index)
	{
		return ptrInternal[index];
	}

	const GenType& operator[](size_t index) const
	{
		return ptrInternal[index];
	}

	FArray<GenType>& operator=(const FArray<GenType> &arr)
	{
		//delete[] ptrInternal;

		size = arr.size;
		capacity = arr.capacity;
		ptrInternal = Memory::Create<GenType>(size);

		memcpy(ptrInternal, arr.ptrInternal, size * sizeof(GenType));

		return *this;
	}


	/* Array Operations */
	void Add(const GenType & v)
	{
		if (size >= capacity)
			Reserve(capacity + 5);
		ptrInternal[size++] = v;
	}


	void Add(GenType && v)
	{
		if (size >= capacity)
			Reserve(capacity + 5);
		ptrInternal[size++] = v;
	}


	void AddArray(const FArray<GenType>&arr)
	{
		size_t oldSize = size;
		Resize(size + arr.size);

		memcpy(ptrInternal + oldSize, arr.ptrInternal, arr.size * sizeof(GenType));
	}


	void Insert(const GenType & elem, size_t index)
	{

		if (index > size || ptrInternal == NULL)
			return;


		GenType* Newbuffer = Memory::Create<GenType>(size + 1);

		memcpy(Newbuffer, ptrInternal, index * sizeof(GenType));
		memcpy(&Newbuffer[index + 1], &ptrInternal[index], (size - index) * sizeof(GenType));

		Newbuffer[index] = elem;

		ptrInternal = Newbuffer;

		capacity = size + 1;
		size++;

	}
	void Insert(const FArray<GenType>&elems, size_t index)
	{

		if (index > size || ptrInternal == NULL || elems.Size() == 0)
			return;


		GenType* Newbuffer = Memory::Create<GenType>(size + elems.size);

		memcpy(Newbuffer, ptrInternal, index * sizeof(GenType));
		memcpy(&Newbuffer[index + elems.size], &ptrInternal[index], (size - index) * sizeof(GenType));

		memcpy(&Newbuffer[index], elems.Begin(), elems.size * sizeof(GenType));

		ptrInternal = Newbuffer;

		capacity = size + elems.size;
		size += elems.size;

	}

	void Remove(size_t index)
	{
		memmove(&ptrInternal[index], &ptrInternal[index + 1], (size-- - index) * sizeof(GenType));

	}

	void Resize(size_t newsize)
	{
		Reserve(newsize);
		size = newsize;
	}


	void Reserve(size_t newcapacity)
	{
		if (ptrInternal == NULL)
		{
			size = 0;
			capacity = 0;
		}

		GenType* Newbuffer = Memory::Create<GenType>(newcapacity);

		unsigned int l_Size = min(newcapacity, size);


		memcpy(Newbuffer, ptrInternal, l_Size * sizeof(GenType));

		capacity = newcapacity;
		//delete[] ptrInternal;
		ptrInternal = Newbuffer;
	}


	void Clear()
	{
		capacity = 0;
		size = 0;
		ptrInternal = NULL;
	}

protected:
	size_t size;
	size_t capacity;
	GenType* ptrInternal;

};
