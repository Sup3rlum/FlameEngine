#include "FLFileStream.h"


using namespace std;

FLFileStream::FLFileStream(STRING path)
{
	ifstream file(path, ios::binary);

	if (file.fail())
	{
		STRING str = "Error opening file " + path;

		MessageBoxA(NULL, str.c_str(), "FlameEngine", MB_OK | MB_ICONERROR);

		return;
	}

	data = std::vector<BYTE>((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());


}


