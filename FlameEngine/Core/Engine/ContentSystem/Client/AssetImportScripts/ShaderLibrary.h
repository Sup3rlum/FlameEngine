#pragma once


#include "../LocalContentService.h"

#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"

EXPORT(struct, FShaderLibSerializer) : public TSerializerInterface<ShaderLibrary>
{
	FShaderLibSerializer(FRIContext * friContext) : renderContext(friContext) {}

	ShaderLibrary Serialize(IOFileStream & fileStream);

	FRIContext* renderContext;
};


struct FShaderLibFileDescriptor : public FLocalContentFileDescriptionBase
{
	FShaderLibFileDescriptor() : FLocalContentFileDescriptionBase("FLSL", ELocalContentFileStorage::BINARY) {}
};


typedef FLocalContentLoader<ShaderLibrary, FShaderLibFileDescriptor, FShaderLibSerializer> FShaderLibLoader;