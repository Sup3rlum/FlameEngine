#pragma once

#include "Core/Math/Module.h"
#include "Core/Engine/GameSystem/Behaviour.h"
#include "Core/Engine/GameSystem/Material/MaterialComponent.h"
#include "Core/Engine/GameSystem/Mesh.h"

class SceneEntityBehaviour : BehaviourScript
{

	void Load() override;
	void Update(float dt) override;

};