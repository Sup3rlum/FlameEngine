#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Behaviour.h"
#include "FlameEngine/Core/Math/Module.h"
#include "FlameEngine/Core/Engine/GameSystem/InputComponent.h"
#include "FlameEngine/Core/Engine/GameSystem/Material/Material.h"
#include "FlameEngine/Core/Engine/GameSystem/Mesh.h"

#include "Core/Engine/ContentSystem/Client/LocalAssetManager.h"

class GameScene;
class FRIContext;


class PlanetScript : public BehaviourScript
{
public:
	float angle = 0.0f;
	PlanetScript(FRIContext* FriContext, GameScene* scene) {}
	void Load() {}
	void Update(float delta)
	{
		//angle += delta;
		AttachedEntity().Transform().Orientation = FQuaternion::FromAxisAngle(angle, FVector3::Normalize(FVector3(-0.3, 1, 0)));
	}
};
