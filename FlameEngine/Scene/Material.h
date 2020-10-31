#pragma once

#include "../dll/nchfx.h"

EXPORT_CLASS Material
{
public:
	DELubyte* colorData;
	DELuint32 colorData_size;

	DELubyte* normalData;
	DELuint32 normalData_size;

	float roughness;
	float specular;
};

