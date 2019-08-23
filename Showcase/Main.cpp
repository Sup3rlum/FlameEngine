

#include "DelusionEngine/Context.h"
#include "DelusionEngine/ContextParameters.h"


int main()
{

	ContextParameters* _contextParams = new ContextParameters();

	_contextParams->contextSampleCount = 4;
	_contextParams->contextMajorVersion = 4;
	_contextParams->contextMinorVersion = 5;
	_contextParams->contextProfile = ContextProfile::OPENGL_CORE;
	_contextParams->contextCompatibility = 1;
	_contextParams->width = 2560;
	_contextParams->height = 1440;

	Context::Initialize(_contextParams);
}