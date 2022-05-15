#include "ShaderLibrary.h"

//#include <iostream>

enum class EShaderIRType : uint32
{
	SPIRV,
	DXIL
};

struct FLibHeader
{
	EShaderIRType Type;
	uint32 ModuleCount;
};



ShaderLibrary TContentSerializer<ShaderLibrary>::Serialize(Stream& fileStream)
{
	ShaderLibrary lib;
	FLibHeader libHeader = fileStream.Read<FLibHeader>();

	for (int i = 0; i < libHeader.ModuleCount; i++)
	{
		FString8 moduleName = fileStream.ReadLengthedString<FString8>();
		uint32 shaderNum = fileStream.Read<uint32>();

		lib.Modules.Set(moduleName, ShaderLibraryModule());
		auto& libModule = lib.Modules[moduleName];


		for (int i = 0; i < shaderNum; i++)
		{
			EFRIShaderType type = fileStream.Read<EFRIShaderType>();
			uint32 byteSize = fileStream.Read<uint32>();	
			FArray<byte> partMemory(byteSize);

			fileStream.Read(partMemory);

			libModule.Parts.Set(type, ShaderLibraryModulePart(partMemory));
		}
	}

	return lib;
}