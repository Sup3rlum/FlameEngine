#pragma once



#include "../AssetManager.h"

#include "Core/Engine/GameSystem/Animation/AnimationComponent.h" 
#include "Core/Engine/FlameRI/FRI.h"

template<>
EXPORT(struct, TContentSerializer<AnimationSequence>)
{

	TContentSerializer() {}

	AnimationSequence Serialize(IOFileStream & fileStream);

};


struct FAnimSequenceFileDescriptor : public FLocalContentFileDescriptionBase
{
	FAnimSequenceFileDescriptor() : FLocalContentFileDescriptionBase("FLAN", ELocalContentFileStorage::BINARY) {}
};
