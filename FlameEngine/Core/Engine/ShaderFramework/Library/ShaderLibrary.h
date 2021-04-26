#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Engine/ContentSystem/Client/LocalContentService.h"


#include "Core/Engine/FlameRI/FRI.h"





EXPORT(struct, ShaderLibraryComponent)
{
	EFRIResourceShaderType shaderType;
	FArray<byte> byteCode;

};

EXPORT(class,  ShaderLibrary)
{
private:
	FRIDynamicAllocator* dynamicAllocator;
public:
	FHashMap<FString, ShaderLibraryComponent> ShaderComponents;



};


struct FShaderLibraryBinarySerializer : TSerializerInterface<ShaderLibrary>
{
	ShaderLibrary Serialize(IOFileStream& fileStream);

	ShaderLibraryComponent& PrecacheShader(FArray<byte> flslByteFormat, EFRIResourceShaderType type);

};
struct FShaderLibraryFileDescriptor : FLocalContentFileDescriptionBase
{
	FShaderLibraryFileDescriptor() : FLocalContentFileDescriptionBase("FSLB", ELocalContentFileStorage::BINARY) {}
};


typedef FLocalContentLoader<ShaderLibrary, FShaderLibraryFileDescriptor, FShaderLibraryBinarySerializer> FShaderLibraryLoader;