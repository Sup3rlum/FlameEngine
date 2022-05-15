#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Runtime/Common/Memory.h"

#include "Stream.h"
#include <fstream>

EXPORT(class, IOFileStream) : public Stream
{
public:
	IOFileStream(FString filepath);
	~IOFileStream();

	void _InternalRead(void* dest, size_t len) override;
	void _InternalWrite(const void* src, size_t len) override;

	size_t ByteSize() const { return 0; }

	FString8 ReadAnsiFile();
	FArray<byte> ReadBytes();

	std::ifstream _file;
};

