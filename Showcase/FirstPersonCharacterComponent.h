#pragma once

#include "FlameEngine/Core/Engine/GameSystem/EntityComponent/EntityComponentSystem.h"

struct FirstPersonCharacterComponent
{
	FVector3 Up;
	FVector3 Right;
	FVector3 LookDirection;
	float flySpeed = 5.0f;
	float pitch;
	float yaw;
	bool isCursorLocked;
};


typedef FEntityComponentSystem<TransformComponent, CameraComponent, FirstPersonCharacterComponent, ControlComponent> FirstPersonCharacterSystem;