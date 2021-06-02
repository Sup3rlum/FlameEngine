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





ShaderLibrary FShaderLibSerializer::Serialize(IOFileStream& fileStream)
{
	ShaderLibrary lib;

	FLibHeader libHeader = fileStream.Read<FLibHeader>();

/*	std::cout << "Loaded shader library:" << std::endl;

	if (libHeader.Type == EShaderIRType::SPIRV)
	{
		std::cout << "Type:SPIRV" << std::endl;
	}
	else if (libHeader.Type == EShaderIRType::DXIL)
	{
		std::cout << "Type:DXIL" << std::endl;
	}
	else
	{
		std::cout << "Type: ERROR " << (int)libHeader.Type << std::endl;
	}

	std::cout << "Modules: "<< libHeader.ModuleCount << std::endl;*/
	
	for (int i = 0; i < libHeader.ModuleCount; i++)
	{
		FAnsiString moduleName = fileStream.ReadLengthedString<FAnsiString>();
		uint32 shaderNum = fileStream.Read<uint32>();

		lib.Modules.Set(moduleName, ShaderLibraryModule());
		auto& libModule = lib.Modules[moduleName];

		/*std::cout << "------- Shader Module ------------" << std::endl;
		std::cout << "Name: " << moduleName.ToPlatformString() << std::endl;
		std::cout << "Parts: " << shaderNum << std::endl;*/

		for (int i = 0; i < shaderNum; i++)
		{
			EFRIResourceShaderType type = fileStream.Read<EFRIResourceShaderType>();
			uint32 byteSize = fileStream.Read<uint32>();	
			FArray<byte> partMemory(byteSize);

			fileStream.ReadArray<byte>(partMemory);

			libModule.Parts.Set(type, ShaderLibraryModulePart(partMemory));

			/*std::cout << "Added Shader with type ";

			if (type == EFRIResourceShaderType::Vertex)
			{
				std::cout << "Vertex";
			}
			else if (type == EFRIResourceShaderType::Pixel)
			{
				std::cout << "Pixel";
			}

			std::cout << " bytesize: " << byteSize << std::endl;*/

		}

	}

	return lib;
}