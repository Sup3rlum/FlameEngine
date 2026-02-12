#include "EditorViewerScript.h"

#include "Core/Engine/ContentSystem/ImportScripts/Common.h"
#include "Core/Engine/GameSystem/CameraSystem/CameraComponent.h"

void EditorViewerScript::Load()
{
	Component<Input>().BindKeyEvent<EditorViewerScript, &EditorViewerScript::KeyEvent>(this);
	Component<Input>().BindMouseEvent<EditorViewerScript, &EditorViewerScript::MouseEvent>(this);
}

void EditorViewerScript::Update(float dt)
{
	auto& control = Component<Input>();
	auto& cam = Component<Camera>();
	auto& transformComponent = Component<FTransform>();
	auto& fps = Component<FPComponent>();


	fps.pitch = FMath::Clamp
	(
		fps.pitch,
		-HALF_PI,
		HALF_PI
	);

	FVector2 cursorPos = FriContext->GetCursorPosition();
	FVector2 viewportSize = FriContext->GetViewport().Size;

	if (control.IsMouseButtonDown(FMouseButton::Left) && cursorPos.x < viewportSize.x && cursorPos.y < viewportSize.y)
	{

		FriContext->SetCursorPosition
		(
			viewportSize / 2.0f
		);

		FVector2 offset = (viewportSize / 2.0f - cursorPos) * 0.3f * -0.003f;

		fps.yaw -= offset.x;
		fps.pitch -= offset.y;
	}


	fps.LookDirection = FVector3
	(
		FMath::Cos(fps.pitch) * FMath::Sin(fps.yaw),
		FMath::Sin(fps.pitch),
		FMath::Cos(fps.pitch) * FMath::Cos(fps.yaw)
	);

	fps.Right = FVector3
	(
		FMath::Sin(fps.yaw - HALF_PI),
		0,
		FMath::Cos(fps.yaw - HALF_PI)
	);

	fps.Up = fps.Right ^ fps.LookDirection;

	UpdateMovement(dt);

	FVector3 viewPos = transformComponent.Position;
	cam.View = FViewMatrix(viewPos, viewPos + fps.LookDirection, fps.Up);
}


void EditorViewerScript::UpdateMovement(float dt)
{
	auto& fps = Component<FPComponent>();
	auto& control = Component<Input>();
	auto& transformComponent = Component<FTransform>();

	FVector3 forward = fps.LookDirection;
	FVector3 right = fps.Right;
	forward = FVector3::Normalize(forward);
	right = FVector3::Normalize(right);

	FVector3 moveVector = 0;

	if (control.IsKeyDown(FKeyboardKeys::A)) moveVector -= right;
	if (control.IsKeyDown(FKeyboardKeys::D)) moveVector += right;
	if (control.IsKeyDown(FKeyboardKeys::W)) moveVector += forward;
	if (control.IsKeyDown(FKeyboardKeys::S)) moveVector -= forward;

	moveVector = FVector3::NormalizeOrZero(moveVector);

	transformComponent.Position += moveVector * dt * 10.0f;
}


void EditorViewerScript::KeyEvent(FKeyboardKeys key, FKeyEvent event)
{
	if (event != FKeyEvent::OnPress)
		return;

}

void EditorViewerScript::MouseEvent(FMouseButton button, FKeyEvent event)
{

}

