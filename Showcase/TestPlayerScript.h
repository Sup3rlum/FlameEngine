#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Behaviour.h"
#include "FlameEngine/Core/Math/Module.h"

class TestGameApplication;
class FRIContext;


struct FPComponent
{
	FVector3 Up;
	FVector3 Right;
	FVector3 LookDirection;
	float flySpeed = 100.0f;
	float pitch;
	float yaw = 2.3f;
	bool isCursorLocked;
};

class TestScript : public BehaviourScript
{
public:

	void Load();
	void Update();

	void UpdateMovement();

	TestScript(FRIContext* FriContext, TestGameApplication* game) : 
		FriContext(FriContext), 
		game(game)
	{}


	FRIContext* FriContext;
	TestGameApplication* game;

	bool wasShooting = false;
	bool isShooting = false;

	bool wasMDown;
	bool wasNDown;

	bool wasSpaceDown = false;
	float UpVelocity = 0;
};