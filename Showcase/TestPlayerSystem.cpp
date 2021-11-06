#include "TestPlayerSystem.h"

#include "TestGame.h";


float playerSpeed = 10.0f;

void TestPlayerSystem::Update(Entity ent, FTransform& transformComponent, CameraComponent& cam, FirstPersonCharacterComponent& fp, ControlComponent& control, CharacterBody& phys)
{
	if (control.IsKeyDown(FKeyboardKeys::Z))
	{
		fp.isCursorLocked = !fp.isCursorLocked;
	}

	if (control.IsKeyDown(FKeyboardKeys::X))
	{
		game->Renderer.isDebug = !game->Renderer.isDebug;
	}



	if (control.IsKeyDown(FKeyboardKeys::M) && !wasMDown)
	{
		
		auto lightng = static_cast<DRStageLighting*>(game->Renderer.RenderStages[4]);
		lightng->Atmosphere->Enabled = !lightng->Atmosphere->Enabled;
	}

	wasMDown = control.IsKeyDown(FKeyboardKeys::M);


	if (control.IsKeyDown(FKeyboardKeys::K))
	{
		game->currentScene->uxContainer->LoadURL("file:///welcome.html");
	}


	FVector3 moveVector = 0;
	FVector3 forward = fp.LookDirection;
	forward.y = 0;


	if (control.IsKeyDown(FKeyboardKeys::A))
	{
		moveVector -= FVector3::Normalize(fp.Right) * playerSpeed;
	}
	if (control.IsKeyDown(FKeyboardKeys::D))
	{

		moveVector += FVector3::Normalize(fp.Right) * playerSpeed;
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


	fp.pitch = FMathFunc::Clamp
	(
		fp.pitch,
		-HALF_PI + 0.1f,
		HALF_PI - 0.1f
	);


	FVector2 cursorPos = FriContext->GetCursorPosition();

	if (fp.isCursorLocked)
	{

		FVector2 viewportSize = FriContext->GetViewport().Size;

		FriContext->SetCursorPosition
		(
			viewportSize / 2.0f
		);


		FVector2 offset = (viewportSize / 2.0f - cursorPos) * 0.3f * -0.003f;

		fp.yaw -= offset.x;
		fp.pitch -= offset.y;
	}


	fp.LookDirection = FVector3
	(
		FMathFunc::Cos(fp.pitch) * FMathFunc::Sin(fp.yaw),
		FMathFunc::Sin(fp.pitch),
		FMathFunc::Cos(fp.pitch) * FMathFunc::Cos(fp.yaw)
	);

	fp.Right = FVector3
	(
		FMathFunc::Sin(fp.yaw - HALF_PI),
		0,
		FMathFunc::Cos(fp.yaw - HALF_PI)
	);

	fp.Up = fp.Right ^ fp.LookDirection;


	float viewHeight = 2 * phys.GetHeight() * 0.8;
	FVector3 viewPos = transformComponent.Position + FVector3(0, viewHeight, 0);

	cam.View = FViewMatrix(viewPos, viewPos + fp.LookDirection, fp.Up);

	wasShooting = isShooting;
}