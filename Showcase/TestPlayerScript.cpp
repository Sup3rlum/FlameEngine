#include "TestPlayerScript.h"
#include "TestGame.h"


void TestScript::Load()
{

	CurrentScene().uxContainer->LoadURL("file:///welcome.html");

	CurrentScene().uxContainer->SetCallback("ExitButton", FDelegate<void(void)>::Make<TestGameApplication, &TestGameApplication::Close>(game));
	CurrentScene().uxContainer->SetCallback("BackButton", FDelegate<void(void)>::Make<TestScript, &TestScript::ReturnToGame>(this));

	AttachedEntity().Component<ControlComponent>().BindKeyEvent<TestScript, &TestScript::KeyEvent>(this);
}

void TestScript::Update(float dt)
{
	auto& control = AttachedEntity().Component<ControlComponent>();
	auto& phys = AttachedEntity().Component<CharacterBody>();
	auto& cam = AttachedEntity().Component<CameraComponent>();
	auto& transformComponent = AttachedEntity().Component<FTransform>();
	auto& fps = AttachedEntity().Component<FPComponent>();


	static float accum = 0;

	accum += dt;
	if (accum > 0.3f)
	{
		FVector3 posVec = AttachedEntity().Component<FTransform>().Position;

		FString8 fpsString = FString8::Format("SetFPS('%0', %1, %2)", 
			1.0f / dt,
			phys.IsGrounded(),
			FVector3(Velocity.x, 0, Velocity.z).Length());

		CurrentScene().uxContainer->ExecuteScript(fpsString);
		accum = 0;
	}
	

	fps.pitch = FMathFunc::Clamp
	(
		fps.pitch,
		-HALF_PI,
		HALF_PI
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

	UpdateMovement(dt);


	float viewHeight = 2 * phys.GetHeight() * 0.8;
	FVector3 viewPos = transformComponent.Position + FVector3(0, viewHeight, 0);

	cam.View = FViewMatrix(viewPos, viewPos + fps.LookDirection, fps.Up);
}


float MaxAirSpeed = 1000.0f;
float MaxGroundSpeed = 1000.0f;

float FrictionFactor = 10.0f;

float AirAcceleration = 50.0f;
float GroundAcceleration = 200.0f;

float GravityAcceleration = 40.0f;
float TerminalVelocity = 30.0f;

float JumpHeight = 2.5f;

FVector3 clampMagnitude(const FVector3& v, float m)
{
	float vLen = v.Length();
	float mag = min(vLen, m);

	FVector3 normVector = 0;
	
	if (vLen > 0.001f)
		normVector = FVector3::Normalize(v);

	return normVector* mag;
}

void TestScript::UpdateMovement(float dt)
{
	auto& fps = AttachedEntity().Component<FPComponent>();
	auto& control = AttachedEntity().Component<ControlComponent>();
	auto& phys = AttachedEntity().Component<CharacterBody>();
	auto& transformComponent = AttachedEntity().Component<FTransform>();

	FVector3 forward = fps.LookDirection;
	FVector3 right = fps.Right;
	forward.y = 0;
	right.y = 0;
	forward = FVector3::Normalize(forward);
	right = FVector3::Normalize(right);


	if (control.IsKeyDown(FKeyboardKeys::Space) && AttachedEntity().Component<CharacterBody>().IsGrounded())
	{
		Jump();
	}

	FVector3 moveVector = 0;

	if (control.IsKeyDown(FKeyboardKeys::A)) moveVector -= right;
	if (control.IsKeyDown(FKeyboardKeys::D)) moveVector += right;
	if (control.IsKeyDown(FKeyboardKeys::W)) moveVector += forward;
	if (control.IsKeyDown(FKeyboardKeys::S)) moveVector -= forward;
	
	moveVector = FVector3::NormalizeOrZero(moveVector);

	if (phys.IsGrounded())
	{
		MoveGround(moveVector, dt);
	}
	else
	{
		MoveAir(moveVector, dt);
	}

	phys.Move(Velocity);
	transformComponent = phys.GetGlobalTransform();
}

void TestScript::ReturnToGame()
{
	AttachedEntity().Component<FPComponent>().isCursorLocked = true;
	settings = false;
	FriContext->HideCursor();



	FVector2 viewportSize = FriContext->GetViewport().Size;
	FriContext->SetCursorPosition
	(
		viewportSize / 2.0f
	);


	CurrentScene().uxContainer->LoadURL("file:///welcome.html");
}

void TestScript::PauseMenu()
{
	AttachedEntity().Component<FPComponent>().isCursorLocked = false;
	settings = true;
	FriContext->ShowCursor();

	CurrentScene().uxContainer->LoadURL("file:///settings.html");


	FString8 fpsString = FString8::Format("setLogs('Stuff')", logString);
	CurrentScene().uxContainer->ExecuteScript(fpsString);

}

void TestScript::KeyEvent(FKeyboardKeys key, FKeyEvent event)
{
	if (event != FKeyEvent::OnPress)
		return;


	auto& fps = AttachedEntity().Component<FPComponent>();

	if (key == FKeyboardKeys::Z && !settings)
	{
		if (fps.isCursorLocked)
		{
			FriContext->ShowCursor();
		}
		else
		{
			FriContext->HideCursor();
		}

		fps.isCursorLocked = !fps.isCursorLocked;

	}
	


	if (key == FKeyboardKeys::Escape)
	{
		if (settings)	ReturnToGame();
		else			PauseMenu();
	}
}


void TestScript::MoveGround(FVector3 moveVector, float dt)
{
	Friction(dt);
	Accelerate(moveVector, GroundAcceleration, MaxGroundSpeed, dt);
}
void TestScript::MoveAir(FVector3 moveVector, float dt)
{
	Gravity(dt);
	Accelerate(moveVector, AirAcceleration, MaxAirSpeed, dt);
}


void TestScript::Friction(float dt)
{
	auto horizontalVel = Velocity;
	horizontalVel.y = 0;

	float gSpeed = horizontalVel.Length();

	if (gSpeed >= 0.00001f)
	{
		float drop = gSpeed * FrictionFactor * dt;
		Velocity *= fmaxf(gSpeed - drop, 0) / gSpeed;
	}
}

void TestScript::Accelerate(FVector3 moveVector, float accel, float maxSpeed, float dt)
{
	auto horizontalVel = Velocity;
	horizontalVel.y = 0;

	auto dVel = moveVector * accel * dt;

	float maxFactor = max(0, 1 - (horizontalVel.Length() / maxSpeed));
	float projVel = FVector3::Dot(horizontalVel, moveVector);

	FVector3 modVel = dVel * maxFactor;
	FVector3 finalVelDelta = FVector3::Lerp(dVel, modVel, projVel);

	Velocity += finalVelDelta;
}


void TestScript::Gravity(float dt)
{
	Velocity.y = fmaxf(Velocity.y - GravityAcceleration * dt, -TerminalVelocity);
}

void TestScript::Jump()
{
	float verticalVelocity = FMathFunc::Sqrt(2 * JumpHeight * GravityAcceleration);
	Velocity.y = verticalVelocity;
}