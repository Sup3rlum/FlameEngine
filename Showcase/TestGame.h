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
	Material rustedMaterial;
	Material caveMaterial;
	Material grassMaterial;
	Material riverMaterial;
	Material metalMaterial;
	Material stuccoMaterial;
	Material graniteMaterial;
	Material tileMaterial;
	Material perfMetalMaterial;
	Material perfPlasticMaterial;
	Material cartoonMaterial;
	Material cliffMaterial;
	Mesh ballMesh;
};

