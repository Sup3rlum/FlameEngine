#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Game.h"



class TestGameApplication : public GameApplication
{
public:
	TestGameApplication(FString name);

	void Load();
	void Dispose();
	void Suspend();



};

