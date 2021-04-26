#pragma once


/*


#include "FlameEngine/Core/Engine/GameSystem/Game.h"
#include "FlameEngine/Core/Engine/GameSystem/PlayerObject.h"
#include "FlameEngine/Core/Engine/GameSystem/Common/SkeletalCharacher.h"

class TestPlayerCharacter : public PlayerObject, public SkeletalCharacher
{
public:
	TestPlayerCharacter(FRIContext* context) :
		FriContext(context),
		isCursorLocked(true),
		yaw(0),
		pitch(0)
	{
		this->ControlComponent.BindKeyEvent<TestPlayerCharacter, &TestPlayerCharacter::PlayerMove>(this);

		this->Transform = FVector3(5, 5, 5);

		playerCamera = new Camera(FViewMatrix(FVector3(5, 5, 5), FVector3(0, 0, 0), FVector3(0, 1, 0)), FPerspectiveMatrix(PI / 4, 16.0f / 9.0f, 0.1f, 100.0f));

	}

	void PlayerMove(FKeyboardKeys keys, FKeyboardKeyEvent keyEvent);
	void Update();


	bool isCursorLocked;

	float mouseSpeed = 0.3f;

	float flySpeed = 1.0f;

	float yaw;
	float pitch;

	FRIContext* FriContext;

	FVector3 LookDirection;
	FVector3 Right;
	FVector3 Up;

	Camera* playerCamera;



};

*/