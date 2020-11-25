#include "EngineInstance.h"




EngineInstance* EngineInstance::_handlingInstance = NULL;

EngineInstance::EngineInstance()
{

	IsAttached = false;

	LoggerService::pHandlingInstance = new LoggerService();

	_handlingInstance = this;

}
FLRESULT EngineInstance::AttachGame(Game* _g)
{
	if (_game == NULL || !IsAttached)
		return FLRESULT::FAIL;

	_game = _g;

	glfwSetKeyCallback(((GLFWContext*)_currentContext)->_windowHandle, key_callback_dispatch);

	_game->_currentScene = new Scene(_currentContext);
	_game->Load();

	return FLRESULT::SUCCESS;
}

FLRESULT EngineInstance::AttachContext(Context* _context)
{
	if (_context == NULL)
	{
		return FLRESULT::FAIL;
	}

	_currentContext = _context;


	RenderState* _default = new RenderState();

	_default->CullState = CullState::Back;
	_default->DepthFunction = DepthFunc::LessOrEqual;
	_default->SourceBlend = BlendFunc::SourceAlpha;
	_default->DestinationBlend = BlendFunc::OneMinusSourceAlpha;


	RenderState::InitilizeSurface();
	RenderState::Push(_default);


	IsAttached = true;

	return FLRESULT::SUCCESS;
}
FLRESULT EngineInstance::Dettach()
{
	if (_currentContext == NULL)
	{
		return FLRESULT::FAIL;
	}

	IsAttached = false;

	_currentContext = NULL;

	return FLRESULT::SUCCESS;
}
FLRESULT EngineInstance::Start()
{

	if (!IsAttached)
	{
		return FLRESULT::FAIL;
	}


	while (!(_currentContext->shouldTerminate) && IsAttached && _game != nullptr)
	{
		_game->_currentScene->Update();
		_game->_currentScene->Render();

		_currentContext->SwapChain();
	}

	return FLRESULT::SUCCESS;
}

void EngineInstance::key_callback_dispatch(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	if (_handlingInstance)
	{
		_handlingInstance->_game->KeyEventCallback(KeyEventArgs((Keys)key, (KeyState)action));
	}
}