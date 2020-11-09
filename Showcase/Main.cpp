

#include "FlameEngine/util/gfunc.h"
#include "TestGame.h"



int main()
{

	ContextDescription _contextDesc;

	_contextDesc.contextSampleCount = 1;
	_contextDesc.contextMajorVersion = 4;
	_contextDesc.contextMinorVersion = 5;
	_contextDesc.contextProfile = ContextProfile::OPENGL_CORE;
	_contextDesc.contextCompatibility = 1;
	_contextDesc.width = 2560;
	_contextDesc.height = 1440;
	_contextDesc.fullscreen = true;

	GLFWContext* context = new GLFWContext();

	context->Initialize(&_contextDesc);

	EngineInstance* _engineInstance = new EngineInstance();

	TestGame* tg = new TestGame("TestGame", DVERSION(1, 0, 0, 0));


	_engineInstance->AttachContext(context);
	_engineInstance->AttachGame(tg);
	_engineInstance->Start();

	return 0;
}