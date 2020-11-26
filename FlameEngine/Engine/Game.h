#pragma once

#include "..\dll\nchfx.h"
#include "..\dll\Version.h"
#include "..\Scene\Scene.h"
#include "..\Input\KeyEventArgs.h"

using namespace std;

EXPORT_CLASS Game
{

public:
	Game(STRING name, DVERSION ver);
	virtual void Load();
	virtual void Dispose();
	virtual void Suspend();



	virtual void KeyEventCallback(KeyEventArgs args);

	STRING Name;
	DVERSION Version;

	Scene* _currentScene;

};

