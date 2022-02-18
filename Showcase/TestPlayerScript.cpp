#include "TestPlayerScript.h"
#include "TestGame.h"

float playerSpeed = 20.0f;


void TestScript::Load()
{

}

void TestScript::Update()
{
	auto& control = AttachedEntity().Component<ControlComponent>();
	auto& phys = AttachedEntity().Component<CharacterBody>();
	auto& cam = AttachedEntity().Component<CameraComponent>();
	auto& transformComponent = AttachedEntity().Component<FTransform>();
	auto& fps = AttachedEntity().Component<FPComponent>();

	if (control.IsKeyDown(FKeyboardKeys::Z))
	{
		fps.isCursorLocked = !fps.isCursorLocked;
	}

	if (control.IsKeyDown(FKeyboardKeys::M) && !wasMDown)
	{
		this->game->Renderer.vxgiRenderer.LightBounces++;
		this->game->Renderer.vxgiRenderer.NeedsUpdate = true;

	}
	if (control.IsKeyDown(FKeyboardKeys::N) && !wasNDown)
	{

		this->game->Renderer.vxgiRenderer.LightBounces--;
		this->game->Renderer.vxgiRenderer.NeedsUpdate = true;
	}

	wasMDown = control.IsKeyDown(FKeyboardKeys::M);
	wasNDown = control.IsKeyDown(FKeyboardKeys::N);


	if (control.IsKeyDown(FKeyboardKeys::K))
	{
		game->currentScene->uxContainer->LoadURL("file:///welcome.html");
	}


	FVector3 moveVector = 0;
	FVector3 forward = fps.LookDirection;
	forward.y = 0;


	if (control.IsKeyDown(FKeyboardKeys::A))
	{
		moveVector -= FVector3::Normalize(fps.Right) * playerSpeed;
	}
	if (control.IsKeyDown(FKeyboardKeys::D))
	{

		moveVector += FVector3::Normalize(fps.Right) * playerSpeed;
	}

	if (control.IsKeyDown(FKeyboardKeys::W))
	{

		moveVector += FVector3::Normalize(forward) * playerSpeed;
	}

	if (control.IsKeyDown(FKeyboardKeys::S))
	{

		moveVector -= FVector3::Normalize(forward) * playerSpeed;
	}

	if (control.IsKeyDown(FKeyboardKeys::Space) && !wasSpaceDown)
	{
		UpVelocity = 30.0f;
	}
	wasSpaceDown = control.IsKeyDown(FKeyboardKeys::Space);

	moveVector += FVector3(0, UpVelocity, 0);
	UpVelocity = fmaxf(UpVelocity - 0.3f, 0);

	phys.Move(moveVector);
	transformComponent = phys.GetGlobalTransform();


	if (control.IsMouseButtonDown(FMouseButton::Left))
	{
		isShooting = true;

	}
	if (!control.IsMouseButtonDown(FMouseButton::Left))
	{
		isShooting = false;
	}
	if (!wasShooting)
	{
		if (isShooting)
		{
			game->Shoot();
		}
	}


	fps.pitch = FMathFunc::Clamp
	(
		fps.pitch,
		-HALF_PI + 0.1f,
		HALF_PI - 0.1f
	);


	FVector2 cursorPos = FriContext->GetCursorPosition();

	if (fps.isCursorLocked)
	{
		FVector2 viewportSize = FriContext->GetViewport().Size;

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
		FMathFunc::Cos(fps.pitch) * FMathFunc::Sin(fps.yaw),
		FMathFunc::Sin(fps.pitch),
		FMathFunc::Cos(fps.pitch) * FMathFunc::Cos(fps.yaw)
	);

	fps.Right = FVector3
	(
		FMathFunc::Sin(fps.yaw - HALF_PI),
		0,
		FMathFunc::Cos(fps.yaw - HALF_PI)
	);

	fps.Up = fps.Right ^ fps.LookDirection;


	float viewHeight = 2 * phys.GetHeight() * 0.8;
	FVector3 viewPos = transformComponent.Position + FVector3(0, viewHeight, 0);

	cam.View = FViewMatrix(viewPos, viewPos + fps.LookDirection, fps.Up);

	wasShooting = isShooting;
}


void TestScript::UpdateMovement()
{

}