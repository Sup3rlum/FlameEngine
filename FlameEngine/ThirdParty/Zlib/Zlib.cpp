#include "Zlib.h"
#include <zlib.h>


FArray<byte> Zlib::Compress(const FArray<byte>& src)
{
	size_t comprUpperBound = compressBound(src.Length());
	byte* destMemory = Memory::Alloc<byte>(comprUpperBound);

	unsigned long destLen;
	compress2(destMemory, &destLen, src.Begin(),src.ByteSize(), 3);

	return FArray<byte>(destMemory, destLen);
}


FArray<byte> Zlib::Decompress(const FArray<byte>& src, size_t outSize)
{
	FArray<byte> destMemory(outSize);

	unsigned long destLen;
	uncompress(destMemory.Begin(), &destLen, src.Begin(), src.ByteSize());

	assert(destLen == destMemory.Length());

	return destMemory;
}
