#include "ShaderLibrary.h"

#include "Core/Framework/IO/FileStream.h"




ShaderLibrary FShaderLibraryBinarySerializer::Serialize(IOFileStream& fileStream)
{
	ShaderLibrary library;

	return library;
}

ShaderLibraryComponent& FShaderLibraryBinarySerializer::PrecacheShader(FArray<byte> flslByteFormat, EFRIResourceShaderType type)
{
	ShaderLibraryComponent component;

	// TODO: Convert FVX shader to respect the respective shading language for the current FRI Context

	
	

	return component;
}
