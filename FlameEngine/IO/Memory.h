#pragma once

#include "../dll/nchfx.h"

EXPORT_CLASS Memory
{
public:
	static float ToSingle(		_In_ BYTE* data);
	static int ToInt(			_In_ BYTE* data);
	static double ToDouble(		_In_ BYTE* data);
	static long long ToLLong(	_In_ BYTE* data);

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

