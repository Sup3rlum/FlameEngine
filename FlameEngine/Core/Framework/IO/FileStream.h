#pragma once

#include "../../flameRT/Memory.h"
#include <fstream>

EXPORT(class, FLFileStream)
{
public:
	FLFileStream(STRING filepath);

	template<typename T>
	T Read()
	{
		T d;// = Memory::ToType<T>(&data[streamPosition]);

		_file.read((char*)&d, sizeof(T));

		return d;
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

private:
	std::ifstream _file;
};

