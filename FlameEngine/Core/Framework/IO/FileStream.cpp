#include "FileStream.h"



IOFileStream::IOFileStream(FString path) : Stream(0)
{
	_file.open(path.ToPlatformString(), std::ios::binary);

	if (_file.fail())
	{
		FString str = FString::Format("Error opening file: %0", path);
		MessageBox(NULL, str.ToPlatformString(), L"FlameEngine", MB_OK | MB_ICONERROR);
		return;
	}

}

void IOFileStream::_InternalRead(void* dest, size_t len)
{
	_file.read((char*)dest, len);
}

void IOFileStream::_InternalWrite(const void* data, size_t len)
{

}

FArray<byte> IOFileStream::ReadBytes()
{
	std::vector<char> contents(
		(std::istreambuf_iterator<char>(_file)),
		(std::istreambuf_iterator<char>()));


	return FArray<byte>(reinterpret_cast<byte*>(&contents[0]), contents.size());
}


FString8 IOFileStream::ReadAnsiFile()
{
	std::string contents(
		(std::istreambuf_iterator<char>(_file)),
		std::istreambuf_iterator<char>());

	return FString8(contents.c_str());
}


IOFileStream::~IOFileStream()
{
	_file.close();
}
