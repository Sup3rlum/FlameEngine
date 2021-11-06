#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Game.h"

#include "FirstPersonCharacterComponent.h"

class TestGameApplication;

class TestPlayerSystem : public FEntityComponentSystem<FTransform, CameraComponent, FirstPersonCharacterComponent, ControlComponent, CharacterBody>
{
public:


	TestPlayerSystem(FRIContext* FriContext, TestGameApplication* game) : FriContext(FriContext), game(game)
	{}

	void Update(Entity ent, FTransform& transformComponent, CameraComponent& cam, FirstPersonCharacterComponent& fp, ControlComponent& control, CharacterBody& phys);

	FRIContext* FriContext;

	bool wasShooting = false;
	bool isShooting = false;

	bool wasMDown;


	bool wasSpaceDown = false;

	TestGameApplication* game;

	float UpVelocity = 0;

};