#include "TestGame.h"


TestGame::TestGame(STRING name, DVERSION dv) : Game(name, dv)
{
	
}

void TestGame::Load()
{

	GameEntity* _monkeyHead = new GameEntity("teapot.demf");


	_currentScene->AddActor("monkey", _monkeyHead);




	Game::Load();
}

void TestGame::Suspend()
{


}

void TestGame::Dispose()
{


}

