#include "TestGame.h"


TestGame::TestGame(STRING name, DVERSION dv) : Game(name, dv)
{
	
}

void TestGame::Load()
{

	_currentScene->LightCollection.push_back(DirectionalLight(fVector3(-1.0f, -1.0f, -1.0f), Color::white, 1.0f));

	GameEntity* _man = new GameEntity("man.fl3d");
	GameEntity* _ground = new GameEntity("plane.fl3d");
	GameEntity* _box = new GameEntity("box.fl3d");


	_man->Position = fVector3(4, -1, -4);
	_box->Position = fVector3(0, 2, 0);
	_ground->Position = fVector3(0, 0, 0);

	_currentScene->AddActor("man", _man);
	_currentScene->AddActor("box", _box);
	_currentScene->AddActor("ground", _ground);
	

	Game::Load();
}

void TestGame::Suspend()
{


}

void TestGame::Dispose()
{


}

