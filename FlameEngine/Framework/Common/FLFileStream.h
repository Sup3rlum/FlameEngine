#pragma once

#include "../../flameRT/Memory.h"

EXPORT(class, FLFileStream)
{
public:
	FLFileStream(STRING filepath);

	template<typename T>
	T Read()
	{
		T d = Memory::ToType<T>(&data[streamPosition]);

		streamPosition += sizeof(T);

		return d;
	}




	template<typename T>
	void ReadArray(_Out_ T* outArray, size_t _Size)
	{
		memcpy(outArray, &data[streamPosition], sizeof(T) * _Size);

		streamPosition += sizeof(T) * _Size;
	}

	template<typename T, size_t _Size>
	void ReadArray(_Out_ T(&outArray)[_Size])
	{
		ReadArray<T>(&outArray[0], _Size);
	}

private:
	std::vector<BYTE> data;
	size_t streamPosition = 0;
};

