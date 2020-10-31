#pragma once

#include "../dll/nchfx.h"

EXPORT_CLASS Memory
{
public:
	static float ToSingle(BYTE* data);
	static int ToInt(BYTE* data);
	static double ToDouble(BYTE* data);
	static long long ToLLong(BYTE* data);

	static void GetData(float f, BYTE* data);
	static void GetData(int i, BYTE* data);
	static void GetData(double d, BYTE* data);
	static void GetData(long long ll, BYTE* data);
};

