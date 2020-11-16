#include "TestGame.h"


#include <immintrin.h>
#include <ctime>

#include "FlameEngine/Mathematics/Types/fVector3.h"
#include "FlameEngine/Mathematics/Types/fMatrix4.h"

TestGame::TestGame(STRING name, DVERSION dv) : Game(name, dv)
{


}

void TestGame::Load()
{

	_currentScene->LightCollection.push_back(DirectionalLight(fVector3(-1.0f, -1.0f, -1.0f), Color::white, 1.0f));

	GameEntity* _man = new GameEntity("man.fl3d");
	GameEntity* _ground = new GameEntity("plane.fl3d");
	GameEntity* _box = new GameEntity("box.fl3d");
	GameEntity* _house = new GameEntity("house.fl3d");


	_man->Position = fVector3(4, -1, -4);
	_box->Position = fVector3(0, 1, 0);
	_ground->Position = fVector3(0, 0, 0);
	_house->Position = fVector3(35, 0.5f, -15);

	_ground->Scale = fVector3(5.0f);
	_box->Scale = fVector3(1.0f);
	_house->Scale = fVector3(0.12f);
	_man->Scale = fVector3(1.0f);

	_currentScene->AddActor("man", _man);
	_currentScene->AddActor("box", _box);
	_currentScene->AddActor("ground", _ground);
	_currentScene->AddActor("house", _house);
	



	Game::Load();
}

void TestGame::Suspend()
{


}

void TestGame::Dispose()
{


}

