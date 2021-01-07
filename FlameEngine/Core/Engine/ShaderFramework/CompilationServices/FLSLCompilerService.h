#pragma once

#include "../Common/Shader.h"
#include "ShaderPreprocessor.h"

EXPORT(class, FLSLCompilerService)
{
public:
	static Shader* CompileShaderFromSourceFile(STRING path, ShaderType shadertype);
	static Shader* CompileShaderFromSource(STRING source, ShaderType shadertype);
};

