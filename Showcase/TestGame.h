#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Game.h"

#include "TestParticleSystem.h"
#include "FlameEngine/Core/Engine/GameSystem/Behaviour.h"

class TestGameApplication : public GameApplication
{
public:
	TestGameApplication(const FString& name);

	void Load();
	void Dispose();
	void Suspend();
	void Update(FGameTime gameTime) override;

	void Close();
	void Shoot();


	Entity playerEntity;

	FProjectionMatrix PerspMatrix;

	Material defaultMaterial;
	Material brickMaterial;
	Material riverRockMaterial;
};

