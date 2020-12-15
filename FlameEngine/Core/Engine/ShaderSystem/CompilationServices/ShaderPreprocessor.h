#pragma once

#include "Core/Common/CoreCommon.h"
#include "../Common/Shader.h"

EXPORT(class, ShaderPreprocessor)
{
public:
	bool Validate(FString data);
	FString RetrieveFull(FString data);

	int GetVersion();
	ShaderType GeShaderType();

private:
	int version;
	ShaderType shaderType;
};

