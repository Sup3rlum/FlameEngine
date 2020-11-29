#pragma once

#include "../dll/nchfx.h"

EXPORT(class,  Memory)
{
public:
	static float ToSingle(				_In_ BYTE* data);
	static int ToInt(					_In_ BYTE* data);
	static double ToDouble(				_In_ BYTE* data);
	static FL_INT64 ToLLong(			_In_ BYTE* data);
	static _UNS_ FL_INT64 ToULLong(		_In_ BYTE* data);
	static STRING ToString(				_In_ BYTE* data, int length);

	static void GetData(float f,		_Out_ BYTE* data);
	static void GetData(int i,			_Out_ BYTE* data);
	static void GetData(double d,		_Out_ BYTE* data);
	static void GetData(long long ll,	_Out_ BYTE* data);


	//TODO: Add memory tracking and release

	template <typename T> 
	static T* Create(int count)
	{
		T* mem = (T*)malloc(sizeof(T) * count);
		return mem;
	}
};

