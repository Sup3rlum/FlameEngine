#pragma once


#include "Core/Common/CoreBase.h"
#include "Core/Common/TypeTraits.h"
#include "Core/Runtime/Common/Memory.h"
#include "Range.h"

template<typename GenType, size_t GenSize>
struct FStaticArray
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
		Memory::CopyCounted<GenType>(dataInternal, constArr, GenSize);
	}
	FStaticArray(const GenType* constArr)
	{
		Memory::CopyCounted<GenType>(dataInternal, constArr, GenSize);
	}
	/*
	*	Copy Constructor
	*/
	FStaticArray(const FStaticArray<GenType, GenSize>&arr)
	{
		Memory::CopyCounted<GenType>(dataInternal, arr.dataInternal, GenSize);
	}
	/*
	*	C++ why you make me use std :(
	*/
	FStaticArray(const std::initializer_list<GenType>& list) : FStaticArray(static_cast<const GenType*>(list.begin())) 
	{
		assert(GenSize == list.size());
	}

	
	virtual ~FStaticArray()
	{
		//Memory::Free(dataInternal);
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
	GenType* Begin()
	{
		return dataInternal;
	}
	const GenType* Begin() const
	{
		return dataInternal;
	}

	GenType* End()
	{
		return dataInternal + GenSize;
	}
	const GenType* End() const
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

	operator GenType* ()
	{
		return dataInternal;
	}

protected:
	GenType dataInternal[GenSize];
};


template<typename GenType>
struct FArray
{
public:

	typedef GenType* IteratorType;


	/*
	*	Default Constructor
	*/
	FArray() :
		size(0),
		capacity(0),
		ptrInternal(NULL)
	{
	}

	FArray(size_t initSize) :
		size(initSize),
		capacity(initSize)
	{
		ptrInternal = Memory::AllocCounted<GenType>(initSize);

		Memory::Zero(ptrInternal, size);
	}
	/*
	*	From memory block Constructor
	*/
	FArray(const GenType* constArr, size_t _Size) :
		size(_Size),
		capacity(_Size)
	{
		ptrInternal = Memory::AllocCounted<GenType>(_Size);

		Memory::CopyCounted<GenType>(ptrInternal, constArr, _Size);
	}
	/*
	*	From FStaticArray constructor
	*/
	template<size_t _Size>
	FArray(const FStaticArray<GenType, _Size>& statarr) : FArray(statarr.Begin(), _Size) {}

	/*
	*	Copy Constructor
	*/
	FArray(const FArray<GenType>& arr) : FArray(arr.ptrInternal, arr.size)
	{

	}
	/*
	*	Move Constructor
	*/
	FArray(FArray<GenType> && arr) :
		size(arr.size),
		capacity(arr.capacity),
		ptrInternal(arr.ptrInternal)
	{

		arr.size = 0;
		arr.capacity = 0;
		arr.ptrInternal = NULL;
	}
	/*
	*	C++ why you make me use std :(
	*/
	FArray(const std::initializer_list<GenType>& list) : FArray()
	{
		for (auto& elem : list)
		{
			this->Add(elem);
		}
	}

	/*
	*   Destructor
	*/

	
	virtual ~FArray()
	{
		Memory::Free(ptrInternal);
	}
	


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
	IteratorType Begin()
	{
		return ptrInternal;
	}
	const IteratorType Begin() const
	{
		return ptrInternal;
	}
	IteratorType End()
	{
		return ptrInternal + size;
	}
	const IteratorType End() const
	{
		return ptrInternal + size;
	}



	/* C++ range based for loop */

	IteratorType begin()
	{
		return ptrInternal;
	}
	const IteratorType begin() const
	{
		return ptrInternal;
	}
	IteratorType end()
	{
		return ptrInternal + size;
	}
	const IteratorType end() const
	{
		return ptrInternal + size;
	}


	/*----------------*/

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
		assert(index < size);

		return ptrInternal[index];
	}

	const GenType& operator[](size_t index) const
	{
		assert(index < size);

		return ptrInternal[index];
	}

	/* Range operators */


	FArray<GenType> operator[](const FRange& range)
	{
		assert(range.end() <= size);
		assert(range.begin() >= 0);

		size_t distance = range.end() - range.begin();
		assert(distance != 0);

		FArray<GenType> rArray;

		for (auto index : range)
		{
			rArray.Add(ptrInternal[index]);
		}

		return rArray;
	}

	/* ------------------------ */


	FArray<GenType>& operator=(const FArray<GenType> &arr)
	{
		//delete[] ptrInternal;

		size = arr.size;
		capacity = arr.capacity;
		ptrInternal = Memory::AllocCounted<GenType>(size);

		Memory::CopyCounted(ptrInternal, arr.ptrInternal, size);

		return *this;
	}


	/* Array Operations */
	FArray<GenType>& Add(const GenType & v)
	{
		if (size >= capacity)
			Reserve(capacity + 5);
		ptrInternal[size++] = v;

		return *this;
	}


	FArray<GenType>& Add(GenType && v)
	{
		if (size >= capacity)
			Reserve(capacity + 5);
		ptrInternal[size++] = MoveRef(v);

		return *this;
	}


	FArray<GenType>& AddArray(const FArray<GenType>&arr)
	{
		size_t oldSize = size;

		if (size + arr.size >= capacity)
			Resize(size + arr.size);
		else
			size += arr.size;

		Memory::CopyCounted(ptrInternal + oldSize, arr.ptrInternal, arr.size);

		return *this;
	}


	FArray<GenType>& Insert(const GenType & elem, size_t index)
	{

		if (index > size || ptrInternal == NULL)
			return *this;


		GenType* Newbuffer = Memory::AllocCounted<GenType>(size + 1);

		Memory::CopyCounted(Newbuffer, ptrInternal, index);
		Memory::CopyCounted(&Newbuffer[index + 1], &ptrInternal[index], (size - index));

		Newbuffer[index] = elem;

		ptrInternal = Newbuffer;

		capacity = size + 1;
		size++;

		return *this;
	}
	FArray<GenType>& Insert(const FArray<GenType>&elems, size_t index)
	{

		if (index > size || ptrInternal == NULL || elems.Length() == 0)
			return *this;


		GenType* Newbuffer = Memory::AllocCounted<GenType>(size + elems.size);

		Memory::CopyCounted(Newbuffer, ptrInternal, index);
		Memory::CopyCounted(&Newbuffer[index + elems.size], &ptrInternal[index], (size - index));

		Memory::CopyCounted(&Newbuffer[index], elems.Begin(), elems.size);

		ptrInternal = Newbuffer;

		capacity = size + elems.size;
		size += elems.size;

		return *this;
	}

	FArray<GenType>& Remove(size_t index)
	{
		Memory::CopyOverlap(&ptrInternal[index], &ptrInternal[index + 1], (size-- - index) * sizeof(GenType));

		return *this;
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

		GenType* Newbuffer = Memory::AllocCounted<GenType>(newcapacity);
		uint32 l_Size = min(newcapacity, size);

		Memory::CopyCounted(Newbuffer, ptrInternal, l_Size);

		capacity = newcapacity;
		Memory::Free(ptrInternal);
		ptrInternal = Newbuffer;
	}

	template<typename TLambda>
	FArray<GenType> Where(TLambda&& lambda) const
	{
		FArray<GenType> result;

		for (const auto& elem : *this)
		{
			if (lambda(elem))
			{
				result.Add(elem);
			}
		}

		return result;
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


#include "FArray.inl"