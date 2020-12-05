#pragma once

#include "../../../dll/nchfx.h"

EXPORT(class, ShaderPreprocessor)
{
public:
	static bool Validate(STRING data);
	static STRING RetrieveFull(STRING data);
};

