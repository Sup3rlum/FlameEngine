#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Game.h"
#include "FlameEngine/Core/Engine/GameSystem/Behaviour.h"
#include "World/World.h"

class FlameCraft : public GameApplication
{
public:
	FlameCraft(const FString& name);

	void Load();
	void Dispose();
	void Suspend();
	void Update(FGameTime gameTime) override;

	void Close();
	void Shoot();

	void CreateParticleSystem();


	World world;
	Entity playerEntity;
	FProjectionMatrix PerspMatrix;

};

