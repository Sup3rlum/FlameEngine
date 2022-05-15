#include "TestPlayerScript.h"
#include "TestGame.h"

#include "FlameEngine/Core/Engine/ContentSystem/ImportScripts/Common.h"

void TestScript::Load()
{

	CurrentScene().uxContainer->LoadURL("file:///welcome.html");

	CurrentScene().uxContainer->SetCallback("ExitButton", FDelegate<void(void)>::Make<TestGameApplication, &TestGameApplication::Close>(game));
	CurrentScene().uxContainer->SetCallback("BackButton", FDelegate<void(void)>::Make<TestScript, &TestScript::ReturnToGame>(this));
	CurrentScene().uxContainer->SetCallback("ClearButton", FDelegate<void(void)>::Make<TestScript, &TestScript::KillBalls>(this));

	CurrentScene().uxContainer->SetCallback("SetFloat", FDelegate<void(float)>::Make<TestScript, &TestScript::SetFloat>(this));

	Component<Input>().BindKeyEvent<TestScript, &TestScript::KeyEvent>(this);
	Component<Input>().BindMouseEvent<TestScript, &TestScript::MouseEvent>(this);


	ballMesh = game->Content.Load<Mesh>("Models/sphere.fl3d");
	ballMaterial = game->Content.Load<Material>("Materials/default2.flmt");
	metal = game->Content.Load<Material>("Materials/perfMetal.flmt");
	plastic = game->Content.Load<Material>("Materials/gold.flmt");
	metal2 = game->Content.Load<Material>("Materials/metal_wall.flmt");
}

void TestScript::SetFloat(float f)
{
	float g = f;


	printf(" Heyyy  %f \n", g);
}

void TestScript::Update(float dt)
{
	auto& control = Component<Input>();
	auto& phys = Component<CharacterBody>();
	auto& cam = Component<Camera>();
	auto& transformComponent = Component<FTransform>();
	auto& fps = Component<FPComponent>();


	static float accum = 0;

	accum += dt;
	if (accum > 0.3f)
	{
		FVector3 posVec = Component<FTransform>().Position;

		FString8 fpsString = FString8::Format("SetFPS('%0', %1, %2, %3, %4)", 
			1.0f / dt,
			phys.IsGrounded(),
			transformComponent.Position.x,
			transformComponent.Position.y,
			transformComponent.Position.z);

		CurrentScene().uxContainer->ExecuteScript(fpsString);
		accum = 0;
	}
	

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

	FVector3 viewPos = phys.GetViewpoint(0.9f);
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
	auto& fps =					Component<FPComponent>();
	auto& control =				Component<Input>();
	auto& phys =				Component<CharacterBody>();
	auto& transformComponent =	Component<FTransform>();

	FVector3 forward = fps.LookDirection;
	FVector3 right = fps.Right;
	forward.y = 0;
	right.y = 0;
	forward = FVector3::Normalize(forward);
	right = FVector3::Normalize(right);


	if (control.IsKeyDown(FKeyboardKeys::Space) && Component<CharacterBody>().IsGrounded())
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
	Component<FPComponent>().isCursorLocked = true;
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
	Component<FPComponent>().isCursorLocked = false;
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


	auto& fps = Component<FPComponent>();

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

void TestScript::MouseEvent(FMouseButton button, FKeyEvent event)
{
	if (button == FMouseButton::Left && event == FKeyEvent::OnPress)
	{
		Shoot();
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

	float maxFactor = FMath::Max(0, 1 - (horizontalVel.Length() / maxSpeed));
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
	float verticalVelocity = FMath::Sqrt(2 * JumpHeight * GravityAcceleration);
	Velocity.y = verticalVelocity;
}


int counter = 0;

void TestScript::Shoot()
{

	Material mats[1] = { metal };

	Entity ball = CurrentScene().CreateEntity<Model, RigidBody>("Ball");

	ball.Component<Model>().Mesh = ballMesh;
	ball.Component<Model>().Material = mats[counter++ % 1];

	auto start = FTransform(Component<CharacterBody>().GetViewpoint(0.9f));
	start.Position += Component<FPComponent>().LookDirection * 1.1f;

	ball.SetComponent<FTransform>(start);
	ball.SetComponent<RigidBody>(CurrentScene().CreateRigidBody(ball.Component<FTransform>()));

	ball.Component<RigidBody>().SetShape(
		PhysShape(
			PhysicsMaterial(0.8f, 0.8f, 0.8f), 
			SphereGeometry(1.0f)
		)
	);

	ball.Component<RigidBody>().SetLinearVelocity(Component<FPComponent>().LookDirection * 100.0f);

}

void TestScript::KillBalls()
{
	auto ents = CurrentScene().QueryEntities("Ball");

	for (auto ent : ents)
	{
		//ent.Kill();
	}
}