

#include "DelusionEngine/ContextManager.h"
#include "DelusionEngine/ContextDescription.h"


int main()
{

	ContextDescription _contextDesc;

	_contextDesc.contextSampleCount = 4;
	_contextDesc.contextMajorVersion = 4;
	_contextDesc.contextMinorVersion = 5;
	_contextDesc.contextProfile = ContextProfile::OPENGL_CORE;
	_contextDesc.contextCompatibility = 1;
	_contextDesc.width = 2560;
	_contextDesc.height = 1440;
	_contextDesc.fullscreen = true;

	ContextManager::CreateNew(_contextDesc);
	ContextManager::Initialize();
}