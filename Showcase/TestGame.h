#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Game.h"

#include "TestPlayerSystem.h"


class TestGameApplication : public GameApplication
{
public:
	TestGameApplication(FString name, EFRIRendererFramework renderFramework);

	void Load();
	void Dispose();
	void Suspend();
	void Update();

	//TestPlayerCharacter* testPlayer;


	void Shoot();


	Entity playerEntity;
	Entity floorEntity;

	Entity boneGuy;
	//FirstPersonCharacterSystem playerSystem;

	FProjectionMatrix PerspMatrix;

	MaterialComponent defaultMaterial;
	MeshComponent ballMesh;

};

