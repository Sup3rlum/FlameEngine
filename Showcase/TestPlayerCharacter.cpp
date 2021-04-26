#include "TestPlayerCharacter.h"



/*
void TestPlayerCharacter::PlayerMove(FKeyboardKeys key, FKeyboardKeyEvent keyEvent)
{
	if (key == FKeyboardKeys::Tab)
	{
		isCursorLocked = !isCursorLocked;
	}
}


void TestPlayerCharacter::Update()
{

	if (ControlComponent.IsKeyDown(FKeyboardKeys::A))
	{
		Transform.Position -= Right * flySpeed * 0.01f;
	}
	if (ControlComponent.IsKeyDown(FKeyboardKeys::D))
	{
		Transform.Position += Right * flySpeed * 0.01f;
	}

	if (ControlComponent.IsKeyDown(FKeyboardKeys::W))
	{
		Transform.Position += LookDirection * flySpeed * 0.01f;
	}

	if (ControlComponent.IsKeyDown(FKeyboardKeys::S))
	{
		Transform.Position -= LookDirection * flySpeed * 0.01f;
	}



	pitch = FMathFunc::Clamp
	(
		pitch, 
		-HALF_PI + 0.1f, 
		HALF_PI - 0.1f
	);


	FVector2 cursorPos = FriContext->GetCursorPosition();

	if (isCursorLocked)
	{

		FVector2 viewportSize = FriContext->GetViewportSize();

		FriContext->SetCursorPosition
		(
			viewportSize / 2.0f
		);


		FVector2 offset = (viewportSize / 2.0f - cursorPos) * mouseSpeed * -0.003f;

		yaw -= offset.x;
		pitch -= offset.y;
	}


	LookDirection = FVector3
	(
		FMathFunc::Cos(pitch) * FMathFunc::Sin(yaw),
		FMathFunc::Sin(pitch),
		FMathFunc::Cos(pitch) * FMathFunc::Cos(yaw)
	);

	Right = FVector3
	(
		FMathFunc::Sin(yaw - HALF_PI),
		0,
		FMathFunc::Cos(yaw - HALF_PI)
	);

	Up = Right ^ LookDirection;


	playerCamera->View = FViewMatrix(Transform.Position, Transform.Position + LookDirection, Up);
}/*/