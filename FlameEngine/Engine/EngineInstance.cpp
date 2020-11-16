#include "EngineInstance.h"

EngineInstance::EngineInstance()
{

	IsAttached = false;


}
DRESULT EngineInstance::AttachGame(Game* _g)
{
	if (_game == NULL || !IsAttached)
		return DRESULT::FAIL;

	_game = _g;

	_game->_currentScene = new Scene(_currentContext);
	_game->Load();

	return DRESULT::SUCCESS;
}

DRESULT EngineInstance::AttachContext(Context* _context)
{
	if (_context == NULL)
	{
		return DRESULT::FAIL;
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

	return DRESULT::SUCCESS;
}
DRESULT EngineInstance::Dettach()
{
	if (_currentContext == NULL)
	{
		return DRESULT::FAIL;
	}

	IsAttached = false;

	_currentContext = NULL;

	return DRESULT::SUCCESS;
}
DRESULT EngineInstance::Start()
{

	if (!IsAttached)
	{
		return DRESULT::FAIL;
	}


	while (!(_currentContext->shouldTerminate) && IsAttached && _game != nullptr)
	{
		_game->_currentScene->Update();
		_game->_currentScene->Render();

		_currentContext->SwapChain();
	}

	return DRESULT::SUCCESS;
}
