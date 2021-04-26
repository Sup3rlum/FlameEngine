#pragma once



#include "../LocalContentService.h"

#include "Core/Engine/GameSystem/Animation/AnimationComponent.h" 
#include "Core/Engine/FlameRI/FRI.h"

EXPORT(struct, FAnimSequenceSerializer) : public TSerializerInterface<AnimationSequence>
{

	FAnimSequenceSerializer() {}

	AnimationSequence Serialize(IOFileStream & fileStream);

};


struct FAnimSequenceFileDescriptor : public FLocalContentFileDescriptionBase
{
	FAnimSequenceFileDescriptor() : FLocalContentFileDescriptionBase("FLAN", ELocalContentFileStorage::BINARY) {}
};


typedef FLocalContentLoader <AnimationSequence, FAnimSequenceFileDescriptor, FAnimSequenceSerializer> FAnimSequenceLoader;