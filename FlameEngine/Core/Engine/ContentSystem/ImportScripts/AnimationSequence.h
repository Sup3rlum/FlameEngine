#pragma once



#include "../AssetManager.h"

#include "Core/Engine/GameSystem/Animation/AnimationComponent.h" 
#include "Core/Engine/FlameRI/FRI.h"

template<>
EXPORT(struct, TContentSerializer<AnimationSequence>)
{

	TContentSerializer(IAssetManager* manager) {}
	AnimationSequence Serialize(Stream & fileStream);

};
