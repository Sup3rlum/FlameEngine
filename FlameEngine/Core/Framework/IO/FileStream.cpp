#include "FileStream.h"



IOFileStream::IOFileStream(FString path)
{
	_file.open(path.ToPlatformString(), std::ios::binary);

	if (_file.fail())
	{
		FString str = FString::Format("Error opening file: %0", path);

		MessageBox(NULL, str.ToPlatformString(), L"FlameEngine", MB_OK | MB_ICONERROR);

		return;
	}

}


