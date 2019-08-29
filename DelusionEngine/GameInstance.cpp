#include "GameInstance.h"

GameInstance::GameInstance(STRING name, DVERSION ver)
{
	Name = name;
	Version = ver;

	IsAttached = false;

}


DRESULT GameInstance::Attach(Context* _context)
{
	if (_context == NULL)
	{
		return DRESULT::FAIL;
	}

	_currentContext = _context;

	_currentScene = new Scene(_context);

	IsAttached = true;

	return DRESULT::SUCCESS;
}
DRESULT GameInstance::Dettach()
{
	if (_currentContext == NULL)
	{
		return DRESULT::FAIL;
	}

	IsAttached = false;

	_currentContext = NULL;

	return DRESULT::SUCCESS;
}
DRESULT GameInstance::Start()
{
	if (!IsAttached)
	{
		return DRESULT::FAIL;
	}

	while (!(_currentContext->shouldTerminate) && IsAttached)
	{
		_currentScene->Update();
		_currentScene->Render();

		_currentContext->SwapChain();
	}

	return DRESULT::SUCCESS;
}