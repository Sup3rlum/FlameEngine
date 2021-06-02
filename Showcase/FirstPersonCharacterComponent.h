#pragma once

#include "FlameEngine/Core/Engine/GameSystem/EntityComponent/EntityComponentSystem.h"

struct FirstPersonCharacterComponent
{
	FVector3 Up;
	FVector3 Right;
	FVector3 LookDirection;
	float flySpeed = 1.0f;
	float pitch;
	float yaw;
	bool isCursorLocked;
};

