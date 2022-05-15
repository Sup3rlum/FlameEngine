#pragma once

#include <FlameEngine/Core/Common/CoreCommon.h>
#include <FlameEngine/Core/Engine/FlameRI/FRI.h>

template<typename _Type, size_t BlockFactor>
struct TMemoryBlock
{
	TMemoryBlock() :
		elems(0),
		capacity(BlockFactor),
		data(NULL)
	{
		data = Memory::AllocCounted<_Type>(BlockFactor);
		Memory::Zero(data, BlockFactor * sizeof(_Type));
	}

	TMemoryBlock(const TMemoryBlock& other) :
		elems(other.elems),
		capacity(other.capacity),
		data(other.data)
	{

	}


	TMemoryBlock& operator=(const TMemoryBlock& other)
	{
		elems = other.elems;
		capacity = other.capacity;
		data =other.data;

		return *this;
	}

	void Write(const _Type& elem)
	{
		if (elems >= capacity)
		{
			auto newCapacity = capacity + BlockFactor;
			auto newData = Memory::AllocCounted<_Type>(newCapacity);
			Memory::Zero(newData, newCapacity * sizeof(_Type));

			Memory::CopyCounted(newData, data, capacity);
			Memory::Free(data);
			data = newData;
			capacity = newCapacity;
		}

		data[elems++] = elem;
	}

	_Type& Top()
	{
		return data[elems - 1];
	}

	void AddElem()
	{
		elems++;
	}

	void Delete()
	{
		Memory::Free(data);
	}

	_Type* Begin()
	{
		return data;
	}

	size_t ByteSize()
	{
		return elems * sizeof(_Type);
	}

	size_t Length()
	{
		return elems;
	}

	FRICreationDescriptor GetFRIDescriptor()
	{
		return FRICreationDescriptor(Begin(), ByteSize());
	}

	_Type* data;
	size_t elems;
	size_t capacity;
};

template<typename _Type>
using FMemoryBlock = TMemoryBlock<_Type, 8192>;
