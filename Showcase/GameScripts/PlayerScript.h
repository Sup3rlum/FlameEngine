#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Behaviour.h"
#include "FlameEngine/Core/Math/Module.h"
#include "FlameEngine/Core/Engine/GameSystem/InputComponent.h"
#include "FlameEngine/Core/Engine/GameSystem/Material/Material.h"
#include "FlameEngine/Core/Engine/GameSystem/Mesh.h"

#include "Core/Engine/ContentSystem/Client/LocalAssetManager.h"

class Scene;
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


class PlayerScript : public BehaviourScript
{
public:

	PlayerScript(FRIContext* FriContext, Scene* scene);
	void Load();
	void Update(float delta);
	void UpdateMovement(float dt);

	void KeyEvent(FKeyboardKeys key, FKeyEvent event);
	void MouseEvent(FMouseButton key, FKeyEvent event);
	void Move();

	void ReturnToGame();
	void PauseMenu();

	FString8 logString = "FlameConsole\r\n_____________________________\r\n";
	FRIContext* FriContext;
	class Scene* scene;

	FVector3 Velocity = 0;
	FVector3 TargetPoint;

	Entity weaponView;

	bool wasOnGround = false;
	bool settings = false;

};