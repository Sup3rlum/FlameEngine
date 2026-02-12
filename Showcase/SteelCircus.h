#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Game.h"
#include "FlameEngine/Core/Engine/GameSystem/Behaviour.h"

#include "Scenes/GameScene.h"
#include "Scenes/SpineScene.h"

class SteelCircus : public GameApplication
{

	Scene* gameScene;

public:
	SteelCircus(const FString& name);

	void Load();
	void Dispose();
	void Suspend();
	void Update(FGameTime gameTime) override;

	void Close();
	void Shoot();
};

