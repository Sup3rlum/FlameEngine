

#include "DelusionEngine/GameInstance.h"
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

	GLFWContext* context = new GLFWContext();

	context->Initialize(&_contextDesc);

	GameInstance* _gameInstance = new GameInstance("Showcase", DVERSION(1, 0, 0, 0));

	_gameInstance->Attach(context);
	_gameInstance->Start();


}