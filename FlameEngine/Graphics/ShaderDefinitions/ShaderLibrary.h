#pragma once

#include "../../dll/nchfx.h"
#include "Shader.h"

EXPORT(class,  ShaderLibrary)
{

	static std::map<STRING, Shader> mInternal;
public:

	static void Load();


};

