#include "ContextManager.h"


BaseContext*		ContextManager::_currentContext		= NULL;
FrameTime*			ContextManager::_frameTime			= NULL;
Scene*				ContextManager::_scene				= NULL;

void ContextManager::CreateNew(ContextDescription _description)
{
	_currentContext = new GLFWContext();
	_currentContext->_contextDescription = _description;
}
void ContextManager::Initialize()
{
	_currentContext->Initialize();

	_frameTime = new FrameTime();
	_scene = new Scene(_currentContext);

	while (!_currentContext->shouldTerminate)
	{
		_frameTime->DeltaTime = glfwGetTime() - _frameTime->TotalTime;
		_frameTime->TotalTime = glfwGetTime();
		_frameTime->EllapsedFrames++;

		//_renderCallback(_frameTime);

		_scene->Render(_frameTime);
		_scene->Update(_frameTime);

		_currentContext->SwapChain();
	}

	_currentContext->Destroy();
}
