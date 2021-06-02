#include "FileStream.h"



IOFileStream::IOFileStream(FString path)
{
	_file.open(path.ToPlatformString(), std::ios::binary);

	if (_file.fail())
	{
		FString str = "Error opening file "; 
		str += path;

		MessageBox(NULL, str.ToPlatformString(), L"FlameEngine", MB_OK | MB_ICONERROR);

		return;
	}

}


