#include "FLFileStream.h"


using namespace std;

FLFileStream::FLFileStream(STRING path)
{
	_file.open(path, ios::binary);

	if (_file.fail())
	{
		STRING str = "Error opening file " + path;

		MessageBoxA(NULL, str.c_str(), "FlameEngine", MB_OK | MB_ICONERROR);

		return;
	}

}


