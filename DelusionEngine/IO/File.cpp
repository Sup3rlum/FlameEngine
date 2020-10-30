#include "File.h"

vector<BYTE> File::ReadAllBytes(STRING path)
{
	ifstream file(path, ios::binary);

	if (file.fail())
	{
		return (vector<BYTE>());
	}

	vector<BYTE> data((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());


	return data;
}