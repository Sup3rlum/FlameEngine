#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Behaviour.h"
#include "FlameEngine/Core/Math/Module.h"
#include "FlameEngine/Core/Engine/GameSystem/ControlComponent.h"

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

	TestScript(FRIContext* FriContext, TestGameApplication* game) :
		FriContext(FriContext),
		game(game)
	{}

	void Load();
	void Update(float delta);
	void UpdateMovement(float delta);

	void MoveGround(FVector3 moveVector, float dt);
	void MoveAir(FVector3 moveVector, float dt);

	void Accelerate(FVector3 moveVector, float accel, float maxSpeed, float dt);
	void Friction(float dt);
	void Gravity(float dt);

	void Jump();

	void KeyEvent(FKeyboardKeys key, FKeyEvent event);

	void ReturnToGame();
	void PauseMenu();

	FString8 logString = "FlameConsole\r\n_____________________________\r\n";
	FRIContext* FriContext;
	TestGameApplication* game;

	FVector3 Velocity = 0;


	bool wasOnGround = false;
	bool settings = false;

};