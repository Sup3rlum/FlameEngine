#pragma once

#include "../dll/nchfx.h"

using namespace std;

EXPORT_CLASS File
{
public:
	static vector<BYTE> ReadAllBytes(STRING path);

};

