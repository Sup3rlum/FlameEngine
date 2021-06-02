#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Game.h"

#include "FirstPersonCharacterComponent.h"

class TestPlayerSystem : public FEntityComponentSystem<FTransform, CameraComponent, FirstPersonCharacterComponent, ControlComponent, CharacterBody>
{
public:


	TestPlayerSystem(FRIContext* FriContext) : FriContext(FriContext)
	{}

	void Update(Entity ent, FTransform& transformComponent, CameraComponent& cam, FirstPersonCharacterComponent& fp, ControlComponent& control, CharacterBody& phys);

	FRIContext* FriContext;

	bool wasShooting = false;
	bool isShooting = false;

};