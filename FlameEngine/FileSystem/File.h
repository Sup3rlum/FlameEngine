#pragma once

#include "../dll/nchfx.h"

using namespace std;

EXPORT(class,  File)
{
public:
	static vector<BYTE> ReadAllBytes(STRING path);

};

