#pragma once

#include "../../dll/nchfx.h"
#include "ShaderSourceDescription.h"

EXPORT_CLASS ShaderSourceLibrary
{

	static std::map<STRING, ShaderSourceDescription> mInternal;
public:

	static void Load();

	static void AddShaderSource(STRING, ShaderSourceDescription);
	static ShaderSourceDescription GetShaderSource(STRING);

};

