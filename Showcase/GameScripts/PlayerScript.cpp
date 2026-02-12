#include "PlayerScript.h"
#include "../SteelCircus.h"
#include "../Scenes/GameScene.h"

#include "FlameEngine/Core/Engine/ContentSystem/ImportScripts/Common.h"


void PlayerScript::Load()
{
	auto& Content = ((SteelCircus*)Game())->Content;


	Component<Input>().BindKeyEvent<PlayerScript, &PlayerScript::KeyEvent>(this);
	Component<Input>().BindMouseEvent<PlayerScript, &PlayerScript::MouseEvent>(this);
}


PlayerScript::PlayerScript(FRIContext* FriContext, Scene* scene) :
	FriContext(FriContext),
	scene(scene)
{

}

void PlayerScript::Update(float dt)
{
	auto& control = Component<Input>();
	//auto& phys = Component<CharacterBody>();
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

	if (fps.isCursorLocked)
	{
		FVector2 viewportSize = FriContext->GetViewport().Size;
		FriContext->SetCursorPosition(viewportSize / 2.0f);

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
	cam.View = FViewMatrix(transformComponent.Position, transformComponent.Position + fps.LookDirection, fps.Up);
}

void PlayerScript::UpdateMovement(float dt)
{
	auto& fps = Component<FPComponent>();
	auto& control = Component<Input>();
	//auto& phys = Component<CharacterBody>();
	auto& transformComponent = Component<FTransform>();

	FVector3 forward = fps.LookDirection;
	FVector3 right = fps.Right;
	FVector3 moveVector = 0;

	if (control.IsKeyDown(FKeyboardKeys::A)) moveVector -= right;
	if (control.IsKeyDown(FKeyboardKeys::D)) moveVector += right;
	if (control.IsKeyDown(FKeyboardKeys::W)) moveVector += forward;
	if (control.IsKeyDown(FKeyboardKeys::S)) moveVector -= forward;

	moveVector = FVector3::NormalizeOrZero(moveVector);
	transformComponent.Position += moveVector * dt;
}


void PlayerScript::ReturnToGame()
{
	settings = false;

	FVector2 viewportSize = FriContext->GetViewport().Size;
	CurrentScene().uxContainer->LoadURL("file:///welcome.html");
}

void PlayerScript::PauseMenu()
{
	settings = true;
	FriContext->ShowCursor();

	CurrentScene().uxContainer->LoadURL("file:///settings.html");

	FString8 fpsString = FString8::Format("setLogs('Stuff')", logString);
	CurrentScene().uxContainer->ExecuteScript(fpsString);
}

void PlayerScript::KeyEvent(FKeyboardKeys key, FKeyEvent event)
{
	if (event != FKeyEvent::OnPress)
		return;

	if (key == FKeyboardKeys::Escape)
	{
		Component<FPComponent>().isCursorLocked = !Component<FPComponent>().isCursorLocked;
	}

	if (key == FKeyboardKeys::G)
	{

	}
}

void PlayerScript::MouseEvent(FMouseButton button, FKeyEvent event)
{

}

