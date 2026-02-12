#pragma once


#include "Core/Common/CoreCommon.h"



class Zlib
{
public:
	/*static void Compress(byte* source, size_t sourceLen, byte* dest, size_t destLen);
	static void Decompress(byte* source, size_t sourceLen, byte* dest, size_t destLen);*/

	static FArray<byte> Compress(const FArray<byte>& src);
	static FArray<byte> Decompress(const FArray<byte>& src, size_t outSize);
};