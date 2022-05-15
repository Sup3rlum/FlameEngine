#include "PlayerScript.h"
#include "FlameCraft.h"
#include "FlameEngine/Core/Engine/ContentSystem/ImportScripts/Common.h"


#include "World/World.h"

void PlayerScript::Load()
{

	CurrentScene().uxContainer->LoadURL("file:///welcome.html");

	CurrentScene().uxContainer->SetCallback("ExitButton", FDelegate<void(void)>::Make<FlameCraft, &FlameCraft::Close>(game));
	CurrentScene().uxContainer->SetCallback("BackButton", FDelegate<void(void)>::Make<PlayerScript, &PlayerScript::ReturnToGame>(this));
	CurrentScene().uxContainer->SetCallback("SetFloat", FDelegate<void(float)>::Make<PlayerScript, &PlayerScript::SetFloat>(this));

	Component<Input>().BindKeyEvent<PlayerScript, &PlayerScript::KeyEvent>(this);
	Component<Input>().BindMouseEvent<PlayerScript, &PlayerScript::MouseEvent>(this);


	ballMesh = game->Content.Load<Mesh>("Models/sphere.fl3d");
	ballMaterial = game->Content.Load<Material>("Materials/default2.flmt");
	metal = game->Content.Load<Material>("Materials/perfMetal.flmt");
	plastic = game->Content.Load<Material>("Materials/gold.flmt");
	metal2 = game->Content.Load<Material>("Materials/metal_wall.flmt");
}

void PlayerScript::SetFloat(float f)
{
	auto val = 2 * PI * f / 1000.0f;
	auto dir = FVector3::Normalize(FVector3(0, sinf(val), cosf(val)));
	auto factor = max(FVector3::Dot(FVector3(0, 1, 0), -dir), 0);


	auto col = FVector3::Lerp(
		FVector3(1.0f, 0.2f, 0),
		FVector3(1.0f, 1.0f, 1.0f),
		factor);
		
	CurrentScene().Elements["Sun"].Component<DirectionalLight>().Direction = dir;
	CurrentScene().Elements["Sun"].Component<DirectionalLight>().Color.rgb = col;

	printf("Atmosphere %f \n", f);
	printf("Factor: %f\n", factor);
}

void PlayerScript::Update(float dt)
{
	auto& control = Component<Input>();
	auto& phys = Component<CharacterBody>();
	auto& cam = Component<Camera>();
	auto& transformComponent = Component<FTransform>();
	auto& fps = Component<FPComponent>();

	static float accum = 0;

	accum += dt;
	if (accum > 0.3f && !settings && !inventory_bool)
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
	UpdateChunks(transformComponent.Position);

	FVector3 viewPos = phys.GetViewpoint(0.9f);
	cam.View = FViewMatrix(viewPos, viewPos + fps.LookDirection, fps.Up);

	_world->Tick();
	_world->playerPos = transformComponent.Position;
	_world->playerLookDir = fps.LookDirection;
}




FVector3 clampMagnitude(const FVector3& v, float m)
{
	float vLen = v.Length();
	float mag = min(vLen, m);

	FVector3 normVector = 0;

	if (vLen > 0.001f)
		normVector = FVector3::Normalize(v);

	return normVector * mag;
}

void PlayerScript::UpdateMovement(float dt)
{
	auto& fps = Component<FPComponent>();
	auto& control = Component<Input>();
	auto& phys = Component<CharacterBody>();
	auto& transformComponent = Component<FTransform>();

	FVector3 forward = fps.LookDirection;
	FVector3 right = fps.Right;
	forward.y = 0;
	right.y = 0;
	forward = FVector3::Normalize(forward);
	right = FVector3::Normalize(right);


	//if (control.IsKeyDown(FKeyboardKeys::Space) && Component<CharacterBody>().IsGrounded())
	if (control.IsKeyDown(FKeyboardKeys::Space))
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

void PlayerScript::ReturnToGame()
{
	Component<FPComponent>().isCursorLocked = true;
	settings = false;
	inventory_bool = false;
	FriContext->HideCursor();



	FVector2 viewportSize = FriContext->GetViewport().Size;
	FriContext->SetCursorPosition
	(
		viewportSize / 2.0f
	);

	//CurrentScene().uxContainer->LoadURL("file:///welcome.html");
	CurrentScene().uxContainer->ExecuteScript("SwitchScreen('ingame')");
	UpdateInventory();

	HoldSlot(holdingSlot);

}

void PlayerScript::PauseMenu()
{
	Component<FPComponent>().isCursorLocked = false;
	settings = true;
	FriContext->ShowCursor();
	//CurrentScene().uxContainer->LoadURL("file:///settings.html");
	CurrentScene().uxContainer->ExecuteScript("SwitchScreen('escape')");

}

void PlayerScript::InventoryMenu()
{
	Component<FPComponent>().isCursorLocked = false;
	inventory_bool = true;
	FriContext->ShowCursor();

	//CurrentScene().uxContainer->LoadURL("file:///inventory.html");
	CurrentScene().uxContainer->ExecuteScript("SwitchScreen('inventory')");


	UpdateInventory();
}


void PlayerScript::UpdateInventory()
{
	FString8 jsArray = inventory[0][0].GetJSString();

	for (int i = 1; i < 36; i++)
	{
		int column = i % 9;
		int row = i / 9;

		jsArray += FString8::Format(",%0", inventory[row][column].GetJSString());
	}

	FString8 fpsString = FString8::Format("SetupInventory2({blocks:[%0]})", jsArray);
	CurrentScene().uxContainer->ExecuteScript(fpsString);
}

void PlayerScript::KeyEvent(FKeyboardKeys key, FKeyEvent event)
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


	if (key == FKeyboardKeys::K)
	{
		CurrentScene().uxContainer->LoadURL("file:///welcome.html");
	}

	if (key == FKeyboardKeys::E)
	{
		if (!settings)
		{	
			if (inventory_bool)
				ReturnToGame();
			else
				InventoryMenu();
		}
	}


	if (key >= FKeyboardKeys::Num1 && key <= FKeyboardKeys::Num9)
	{
		int slot = (int)key - (int)FKeyboardKeys::Num1;
		HoldSlot(slot);
	}

	if (key == FKeyboardKeys::H)
	{
		UpdateInventory();
	}

}


void PlayerScript::HoldSlot(int slot)
{ 
	holdingSlot = slot;

	FString8 fpsString = FString8::Format("Hold(%0)", slot);
	CurrentScene().uxContainer->ExecuteScript(fpsString);
}

void PlayerScript::MouseEvent(FMouseButton button, FKeyEvent event)
{
	if (button == FMouseButton::Left && event == FKeyEvent::OnPress)
	{
		RemoveBlock();
	}
	if (button == FMouseButton::Right && event == FKeyEvent::OnPress)
	{
		PlaceBlock();
	}
	if (button == FMouseButton::Middle && event == FKeyEvent::OnPress)
	{
		PickBlock();
	}

}

void PlayerScript::MoveGround(FVector3 moveVector, float dt)
{
	Friction(dt);
	Accelerate(moveVector, GroundAcceleration, MaxGroundSpeed, dt);
}
void PlayerScript::MoveAir(FVector3 moveVector, float dt)
{
	Gravity(dt);
	Accelerate(moveVector, AirAcceleration, MaxAirSpeed, dt);
}


void PlayerScript::Friction(float dt)
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

void PlayerScript::Accelerate(FVector3 moveVector, float accel, float maxSpeed, float dt)
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


void PlayerScript::Gravity(float dt)
{
	Velocity.y = fmaxf(Velocity.y - GravityAcceleration * dt, -TerminalVelocity);
}

void PlayerScript::Jump()
{
	float verticalVelocity = FMath::Sqrt(2 * JumpHeight * GravityAcceleration);
	Velocity.y = verticalVelocity;
}


int counter = 0;

void PlayerScript::Shoot()
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


int renderDistance = 16;


void PlayerScript::UpdateChunks(FVector3 pos)
{
	int generatedChunks = 0;

	for (auto xChunk : FRange(-renderDistance, renderDistance))
	{
		for (auto zChunk : FRange(-renderDistance, renderDistance))
		{
			IVector3 blockPos = pos;
			auto chunkToLoad = World::GlobalToChunkCoord(blockPos) + IVector2(xChunk, zChunk);

			if (!_world->ChunkData.Contains(chunkToLoad))
			{
				_world->GenerateChunk(chunkToLoad);
				generatedChunks++;
			}
		}
	}

	if (generatedChunks)
		printf("Scheduled %d chunks\n", generatedChunks);
}


int placeDistance = 20;
int iterations = 10;


IVector3 IntersectionNormal(FVector3 origin, FVector3 dir)
{
	auto t1 = (- origin[0]) / dir[0];
	auto t2 = (1.0f - origin[0]) / dir[0];

	auto tmin = min(t1, t2);
	auto tmax = max(t1, t2);

	for (int i = 1; i < 3; ++i)
	{
		t1 = (-origin[i]) / dir[i];
		t2 = (1.0f - origin[i]) / dir[i];

		tmin = max(tmin, min(t1, t2));
		tmax = min(tmax, max(t1, t2));
	}

	FVector3 intersection = origin + dir * tmax;

	if (intersection.x == 1.0f) return IVector3(1, 0, 0);
	if (intersection.x == 0.0f) return IVector3(-1, 0, 0);
	if (intersection.y == 1.0f) return IVector3(0, 1, 0);
	if (intersection.y == 0.0f) return IVector3(0, -1, 0);
	if (intersection.z == 1.0f) return IVector3(0, 0, 1);
	if (intersection.z == 0.0f) return IVector3(0, 0, -1);

	return IVector3(0, 1, 0);
}


void PlayerScript::PlaceBlock()
{
	auto& fp = Component<FPComponent>();
	auto origin = Component<CharacterBody>().GetViewpoint(0.9f);
	auto accum = origin + fp.LookDirection;

	for (int i = 0; i < placeDistance * iterations; i++)
	{
		IVector3 blockPos = accum;

		if (accum.x < 0) blockPos.x--;
		if (accum.z < 0) blockPos.z--;

		if (_world->GetBlock(blockPos).ID > 0)
		{
			FVector3 blockPosFloat = blockPos;
			auto origin = accum - blockPosFloat;
			auto dir = -fp.LookDirection;

			auto normal = IntersectionNormal(origin, dir);
			auto placeBlock = blockPos + normal;

			if (_world->GetBlock(placeBlock).ID == 0 && 
				inventory[0][holdingSlot].block.ID > 0 &&
				inventory[0][holdingSlot].amount > 0
				)
			{
				_world->SetBlock(inventory[0][holdingSlot].block, blockPos + normal);
				inventory[0][holdingSlot].amount--;

				if (!inventory[0][holdingSlot].amount)
					inventory[0][holdingSlot].block = Block(0);

				UpdateInventory();

			}

			break;
		}
		accum += fp.LookDirection / (float)iterations;
	}
}

void PlayerScript::RemoveBlock()
{
	auto& fp = Component<FPComponent>();
	auto origin = Component<CharacterBody>().GetViewpoint(0.9f);
	auto accum = origin + fp.LookDirection;

	for (int i = 0; i < placeDistance * iterations; i++)
	{
		IVector3 blockPos = accum;

		if (accum.x < 0) blockPos.x--;
		if (accum.z < 0) blockPos.z--;

		auto _block = _world->GetBlock(blockPos);

		if (_block.ID > 0)
		{
			_world->SetBlock(Block(0), blockPos);


			bool foundStack = false;

			for (int s = 0; s < 36; s++)
			{
				int column = s % 9;
				int row = s / 9;

				if (inventory[row][column].block.ID == _block.ID)
				{
					inventory[row][column].amount++;
					foundStack = true;
					break;
				}
			}

			if (!foundStack)
			{
				for (int s = 0; s < 36; s++)
				{
					int column = s % 9;
					int row = s / 9;

					if (inventory[row][column].block.ID == 0)
					{
						inventory[row][column].block = _block;
						inventory[row][column].amount = 1;
						break;
					}
				}
			}

			UpdateInventory();

			break;
		}
		accum += fp.LookDirection / (float)iterations;
	}
}

void PlayerScript::PickBlock()
{
	auto& fp = Component<FPComponent>();
	auto origin = Component<CharacterBody>().GetViewpoint(0.9f);
	auto accum = origin + fp.LookDirection;

	for (int i = 0; i < placeDistance * iterations; i++)
	{
		IVector3 blockPos = accum;

		if (accum.x < 0) blockPos.x--;
		if (accum.z < 0) blockPos.z--;

		auto _block = _world->GetBlock(blockPos);

		if (_block.ID > 0)
		{
			if (inventory[0][holdingSlot].block.ID == _block.ID)
			{
				inventory[0][holdingSlot].amount++;
			}
			else
			{
				inventory[0][holdingSlot].block = _block;
				inventory[0][holdingSlot].amount = 1;
			}


			UpdateInventory();

			break;
		}
		accum += fp.LookDirection / (float)iterations;
	}
}