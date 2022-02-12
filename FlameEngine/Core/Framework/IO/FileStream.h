#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Runtime/Common/Memory.h"
#include <fstream>

EXPORT(class, IOFileStream)
{
public:
	IOFileStream(FString filepath);

	template<typename T>
	T Read()
	{
		T d;// = Memory::ToType<T>(&data[streamPosition]);

		_file.read((char*)&d, sizeof(T));

		return d;
	}

	template<typename GenString>
	GenString ReadLengthedString()
	{
		GenString out;
		uint32 length = Read<uint32>();

		out.Resize(length);

		ReadArray(out.Begin(), length * sizeof(typename GenString::CharType));

		return out;
	}


	template<typename TType>
	FArray<TType> ReadArray(size_t elementCount)
	{
		FArray<TType> out(elementCount);

		_file.read((char*)out.Begin(), sizeof(TType) * elementCount);

		return out;
	}

	template<typename TGen>
	void ReadArray(FArray<TGen>& buffer)
	{
		_file.read((char*)buffer.Begin(), buffer.ByteSize());
	}


	template<typename T>
	void ReadArray(_Out_ T* outArray, size_t _Size)
	{
		//memcpy(outArray, &data[streamPosition], sizeof(T) * _Size);

		_file.read((char*)outArray, sizeof(T) * _Size);

	}


	template<typename T, size_t _Size>
	void ReadArray(_Out_ T(&outArray)[_Size])
	{
		ReadArray<T>(&outArray[0], _Size);
	}


	template<typename T, size_t _Size>
	void ReadArray(_Out_ FStaticArray<T, _Size>& outArray)
	{
		ReadArray<T>(&outArray[0], _Size);
	}


	FAnsiString ReadAnsiFile()
	{
		std::string contents((std::istreambuf_iterator<char>(_file)),
			std::istreambuf_iterator<char>());

		;

		return FAnsiString(contents.c_str());
	}


	FArray<byte> ReadBytes()
	{
		std::vector<char> contents(
			(std::istreambuf_iterator<char>(_file)),
			(std::istreambuf_iterator<char>()));


		return FArray<byte>(reinterpret_cast<byte*>(&contents[0]), contents.size());
	}

	~IOFileStream()
	{
		_file.close();
	}

	std::ifstream _file;
};

