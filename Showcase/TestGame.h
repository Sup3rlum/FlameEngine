#pragma once

#include "DelusionEngine\Context\GLFWContext.h"
#include "DelusionEngine\Engine\EngineInstance.h"
#include "DelusionEngine\Scene\GameEntity.h"



class TestGame : public Game
{
public:
	TestGame(STRING name, DVERSION dv);

	void Load();
	void Dispose();
	void Suspend();
};

