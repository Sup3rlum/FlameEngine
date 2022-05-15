#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Behaviour.h"
#include "FlameEngine/Core/Math/Module.h"
#include "FlameEngine/Core/Engine/GameSystem/InputComponent.h"
#include "FlameEngine/Core/Engine/GameSystem/Material/MaterialComponent.h"
#include "FlameEngine/Core/Engine/GameSystem/Mesh.h"

#include "World/Block.h"

class FlameCraft;
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


struct InventorySlot
{
	Block block;
	int amount;

	FString8 GetJSString()
	{
		return FString8::Format("{id:%0,count:%1}", block.ID, amount);
	}

};


class PlayerScript : public BehaviourScript
{
public:

	PlayerScript(FRIContext* FriContext, FlameCraft* game) :
		FriContext(FriContext),
		game(game)
	{}

	void Load();
	void Update(float delta);
	void UpdateMovement(float delta);
	void UpdateChunks(FVector3 pos);

	void PlaceBlock();
	void RemoveBlock();
	void PickBlock();
	void MoveGround(FVector3 moveVector, float dt);
	void MoveAir(FVector3 moveVector, float dt);

	void Accelerate(FVector3 moveVector, float accel, float maxSpeed, float dt);
	void Friction(float dt);
	void Gravity(float dt);

	void Jump();
	void Shoot();

	void SetFloat(float f);

	void KeyEvent(FKeyboardKeys key, FKeyEvent event);
	void MouseEvent(FMouseButton key, FKeyEvent event);

	void ReturnToGame();
	void PauseMenu();
	void InventoryMenu();

	void HoldSlot(int slot);
	void UpdateInventory();

	FRIContext* FriContext;
	FlameCraft* game;

	FVector3 Velocity = 0;
	Mesh ballMesh;
	Material ballMaterial;
	Material metal;
	Material plastic;
	Material metal2;

	bool wasOnGround = false;
	bool settings = false;
	bool inventory_bool = true;


	InventorySlot inventory[4][9] = { 0 };
	int holdingSlot = 0;
	

	class World* _world;

	float MaxAirSpeed = 1000.0f;
	float MaxGroundSpeed = 1000.0f;
	float FrictionFactor = 10.0f;
	float AirAcceleration = 50.0f;
	float GroundAcceleration = 200.0f;
	float GravityAcceleration = 40.0f;
	float TerminalVelocity = 30.0f;
	float JumpHeight = 2.5f;

};