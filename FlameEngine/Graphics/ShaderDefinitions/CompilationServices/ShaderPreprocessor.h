#pragma once

#include "../../../dll/nchfx.h"
#include "../Shader.h"

EXPORT(class, ShaderPreprocessor)
{
public:
	bool Validate(STRING data);
	STRING RetrieveFull(STRING data);

	int GetVersion();
	ShaderType GeShaderType();

private:
	int version;
	ShaderType shaderType;
};

