#pragma once

#include "nchfx.h"

EXPORT_STRUCT DVERSION
{
public:
	DVERSION(int maj, int min, int rev, int build);
	DVERSION();

	int major;
	int minor;
	int revision;
	int build;

	std::string ToString();
	std::string ToString(char delim);
	int ToInt();
};