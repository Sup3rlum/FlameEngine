#pragma once


#include "FlameEngine/Core/Math/Module.h"
#include "FlameEngine/Core/Engine/GameSystem/Behaviour.h"
#include "FlameEngine/Core/Engine/GameSystem/Material/MaterialComponent.h"
#include "FlameEngine/Core/Engine/GameSystem/Mesh.h"

class TestGameApplication;

class CoinScript : public BehaviourScript
{
public:

	CoinScript(TestGameApplication* game) : game(game) {}

	void Load() override
	{
	}

	void Update(float dt) override;

	void Relocate();

	TestGameApplication* game;
};