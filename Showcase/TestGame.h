#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Game.h"

#include "FirstPersonCharacterComponent.h"


class TestGameApplication : public GameApplication
{
public:
	TestGameApplication(FString name);

	void Load();
	void Dispose();
	void Suspend();
	void Update();

	//TestPlayerCharacter* testPlayer;


	void Shoot();

	bool wasShooting = false;
	bool isShooting = false;

	Entity playerEntity;
	Entity floorEntity;

	Entity boneGuy;
	//FirstPersonCharacterSystem playerSystem;
};

