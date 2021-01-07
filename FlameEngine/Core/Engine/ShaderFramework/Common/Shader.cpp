#include "Shader.h"


Shader::~Shader()
{
	glDeleteShader(handle);
}