#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Game.h"
#include "FlameEngine/Core/Engine/GameSystem/Behaviour.h"


class GameScene : public Scene
{
public:
	FMatrix4 PerspMatrix;
	void OnCreateResources(FRIContext* FriContext, FAssetManager& Content);
	void OnDestroyResources();
	void Update(float dt);

	GameScene(class GameApplication* Game, FRIContext* context, PhysicsDescription pDesc) : Scene("GameScene", Game, context, pDesc)
	{

	}
};