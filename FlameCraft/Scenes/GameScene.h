#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Game.h"
#include "FlameEngine/Core/Engine/GameSystem/Behaviour.h"
#include "../World/World.h"

class GameScene : public Scene
{
public:
	World world;
	Entity playerEntity;
	FProjectionMatrix PerspMatrix;

	void OnCreateResources(FRIContext* FriContext, FAssetManager& Content);
	void OnDestroyResources();

	GameScene(class GameApplication* Game, FRIContext* context, PhysicsDescription pDesc) : Scene("GameScene", Game, context, pDesc)
	{

	}
};