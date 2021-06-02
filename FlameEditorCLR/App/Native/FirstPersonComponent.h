#pragma once

#include "Core/Engine/GameSystem/EntityComponent/EntityComponentSystem.h"
#include "Core/Math/Module.h"

namespace FlameEditorCLR
{

	struct FirstPersonComponent
	{
		FVector3 Up;
		FVector3 Right;
		FVector3 LookDirection;
		float flySpeed = 5.0f;
		float pitch;
		float yaw;
		bool isCursorLocked;
	};
}