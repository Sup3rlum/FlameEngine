#include "ShaderPreprocessor.h"


bool ShaderPreprocessor::Validate(STRING data)
{
	return true;
}

STRING ShaderPreprocessor::RetrieveFull(STRING data)
{
	return data;
}

int ShaderPreprocessor::GetVersion()
{
	return version;
}
ShaderType ShaderPreprocessor::GeShaderType()
{
	return shaderType;
}