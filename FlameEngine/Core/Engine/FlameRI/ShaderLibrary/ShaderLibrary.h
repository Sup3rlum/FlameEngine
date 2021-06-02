#pragma once


#include "Core/Common/CoreCommon.h"


struct ShaderLibraryModulePart
{
	FArray<byte> Memory;

	ShaderLibraryModulePart()
	{

	}

	ShaderLibraryModulePart(const FArray<byte>& memory) :
		Memory(memory)
	{
	}

	ShaderLibraryModulePart(const ShaderLibraryModulePart& other) :
		Memory(other.Memory)
	{
	}
	ShaderLibraryModulePart& operator=(const ShaderLibraryModulePart& other)
	{
		Memory = other.Memory;

		return *this;
	}


	~ShaderLibraryModulePart()
	{

	}
};

class ShaderLibraryModule
{
public:

	ShaderLibraryModule()
	{
	}

	ShaderLibraryModule(const ShaderLibraryModule& other) : Parts(other.Parts)
	{
	}

	ShaderLibraryModule& operator=(const ShaderLibraryModule& other)
	{
		for (const auto& kv : other.Parts)
		{
			Parts.Set(kv);
		}

		return *this;
	}

	FHashMap<EFRIResourceShaderType, ShaderLibraryModulePart> Parts;
};

class ShaderLibrary
{
public:
	FHashMap<FAnsiString, ShaderLibraryModule> Modules;

	ShaderLibrary()
	{

	}

	ShaderLibrary(const ShaderLibrary& other) : Modules(other.Modules)
	{

	}

	ShaderLibrary& operator=(const ShaderLibrary& other)
	{
		Modules = other.Modules;

		return *this;
	}


};