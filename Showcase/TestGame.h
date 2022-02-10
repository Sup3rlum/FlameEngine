#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Game.h"

#include "TestPlayerSystem.h"
#include "TestParticleSystem.h"


class TestGameApplication : public GameApplication
{
public:
	TestGameApplication(FString name, EFRIRendererFramework renderFramework);

	void Load();
	void Dispose();
	void Suspend();
	void Update(FGameTime gameTime);


	void Shoot();

	float angle = 0;


	Entity playerEntity;
	Entity floorEntity;

	Entity benchEntity;

	Entity boneGuy;


	FProjectionMatrix PerspMatrix;

	Material defaultMaterial;
	Material brickMaterial;
	Material riverRockMaterial;

	Mesh ballMesh;
};

