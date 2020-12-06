#include "FLSLCompilerService.h"


Shader* FLSLCompilerService::CompileShaderFromSourceFile(STRING path, ShaderType shadertype)
{
	std::ifstream sourceFile(path);

	if (sourceFile.fail())
	{
		STRING str = "Error opening file " + path;

		MessageBoxA(NULL, str.c_str(), "FlameEngine", MB_OK | MB_ICONERROR);

		return NULL;
	}


	std::string sourceCode((std::istreambuf_iterator<char>(sourceFile)), std::istreambuf_iterator<char>());

	return CompileShaderFromSource(sourceCode, shadertype);
}

Shader* FLSLCompilerService::CompileShaderFromSource(STRING source, ShaderType shadertype)
{
	Shader* shader = new Shader(shadertype);


	const char* vCstr = source.c_str();

	shader->handle = glCreateShader((GLuint)shadertype);
	glShaderSource(shader->handle, 1, &vCstr, NULL);
	glCompileShader(shader->handle);


	int vSuccess;
	char vInfo[512];

	glGetShaderiv(shader->handle, GL_COMPILE_STATUS, &vSuccess);
	if (!vSuccess)
	{
		glGetShaderInfoLog(shader->handle, 512, NULL, vInfo);
		FLAME_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + STRING(vInfo));
	};

	return shader;
}