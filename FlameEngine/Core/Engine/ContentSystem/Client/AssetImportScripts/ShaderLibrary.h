#pragma once


#include "../LocalContentService.h"

#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"

template<>
EXPORT(struct, TContentSerializer<ShaderLibrary>)
{
	TContentSerializer(FRIContext * friContext) : renderContext(friContext) {}
	ShaderLibrary Serialize(IOFileStream & fileStream);

	FRIContext* renderContext;
};


struct FShaderLibFileDescriptor : public FLocalContentFileDescriptionBase
{
	FShaderLibFileDescriptor() : FLocalContentFileDescriptionBase("FLSL", ELocalContentFileStorage::BINARY) {}
};
