// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include "Context.h"

int main() 
{
	ContextParameters* _contextParams = new ContextParameters();

	_contextParams->contextSampleCount = 4;
	_contextParams->contextMajorVersion = 4;
	_contextParams->contextMinorVersion = 5;
	_contextParams->contextProfile = GLFW_OPENGL_CORE_PROFILE;
	_contextParams->contextCompatibility = GL_TRUE;
	_contextParams->width = 2560;
	_contextParams->height = 1440;

	Context::Initialize(_contextParams);
}