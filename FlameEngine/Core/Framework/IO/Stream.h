#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Runtime/Common/Memory.h"


class Stream
{
public:
	virtual void _InternalRead(void* dest, size_t len) = 0;
	virtual void _InternalWrite(const void* src, size_t len) = 0;

	virtual size_t ByteSize() const = 0;

	void WriteTo(Stream& stream)
	{
		void* Buffer = Memory::Alloc<void>(ByteSize());
		Position = 0;
		_InternalRead(Buffer, ByteSize());
		stream._InternalWrite(Buffer, ByteSize());
		Memory::Free(Buffer);
	}

	template<typename TType>
	FArray<TType> ReadArray(size_t elementCount)
	{
		FArray<TType> out(elementCount);
		Read(out.Begin(), out.ByteSize());
		return out;
	}

	template<typename TType>
	TType Read()
	{
		TType object;
		_InternalRead(&object, sizeof(TType));
		return object;
	}

	template<typename TGen>
	void Read(FArray<TGen>& buffer)
	{
		_InternalRead(buffer.Begin(), buffer.ByteSize());
	}


	template<typename T, size_t _Size>
	void Read(_Out_ T(&outArray)[_Size])
	{
		_InternalRead(&outArray[0], _Size * sizeof(T));
	}


	template<typename T, size_t _Size>
	void Read(_Out_ FStaticArray<T, _Size>& outArray)
	{
		_InternalRead(outArray.Begin(), outArray.ByteSize());
	}

	void Read(void* data, size_t byteSize)
	{
		_InternalRead(data, byteSize);
	}

	template<typename GenString>
	GenString ReadLengthedString()
	{
		uint32 length = Read<uint32>();

		GenString out;
		out.Resize(length);
		_InternalRead(out.Begin(), out.ByteSize());

		return out;
	}

	template<typename TGen>
	void Write(const FArray<TGen>& inArray)
	{
		_InternalWrite(inArray.Begin(), inArray.ByteSize());
	}

	template<typename TGen, size_t _Size>
	void Write(const FStaticArray<TGen, _Size>& inArray)
	{
		_InternalWrite(inArray.Begin(), inArray.ByteSize());
	}

	template<typename TType>
	void Write(const TType& object)
	{
		_InternalWrite(&object, sizeof(TType));
	}

protected:
	Stream(size_t capacity) :
		Capacity(capacity),
		Position(0),
		Length(0)
	{
	}

	virtual ~Stream()
	{
	}

	size_t Position;
	size_t Capacity;
	size_t Length;
};

class ByteStream : public Stream
{
public:
	ByteStream() : Stream(2048)
	{
		data = Memory::Alloc<byte>(2048);
	}
protected:
	void _InternalRead(_Out_ void* dest, size_t count) override
	{
		if (Position + count < Length)
		{
			Memory::Copy<void>(dest, &data[Position], count);
			Position += count;
		}
	}

	void _InternalWrite(const void* Data, size_t count) override
	{
		EnsureCapacity(Position + count);
		Memory::Copy<void>(&data[Position], Data, count);

		Length += max(0, Position + 1 + count - Length);
		Position += count;
	}

	void EnsureCapacity(size_t newCapacity)
	{
		if (newCapacity >= Capacity)
		{
			auto newData = Memory::Alloc<byte>(newCapacity + 512);
			Memory::Copy(newData, data, Capacity);
			Memory::Free(data);

			data = newData;
		}
	}

public:
	const void* Begin() const
	{
		return data;
	}

	void* Begin()
	{
		return data;
	}

	size_t ByteSize() const
	{
		return Length;
	}

	FArray<byte> ToBytes()
	{
		return FArray<byte>(data, Length/ sizeof(byte));
	}

	void SetPosition(size_t pos)
	{
		Position = pos;
	}


	virtual ~ByteStream()
	{
		Memory::Free(data);
	}

private:
	byte* data;
};