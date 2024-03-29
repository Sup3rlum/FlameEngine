#pragma once

#include "../../FlameRI/FRI.h"
#include "../EntityComponent/EntityComponent.h"
#include "MaterialDefinitions.h"


enum class EMaterialMap : uint32
{
	Diffuse,
	Normal,
	Detail,
	Emissive,
	MAX_MAPS
};


struct MaterialMap
{
	FRITexture2D* Handle;

	MaterialMap(FRITexture2D* Handle) : Handle(Handle) {}
	MaterialMap() : Handle(NULL) {}
};

struct MaterialProperties
{
	FRIBool HasNormalMap;
	FRIBool HasPOMEnabled;
	FRIBool HasAOMap;
	FRIBool HasEmissive;
	FRIBool HasTransluscent;

	MaterialProperties()
	{
		HasNormalMap = EFRIBool::True;
		HasPOMEnabled = EFRIBool::True;
		HasAOMap = EFRIBool::True;
		HasEmissive = EFRIBool::False;
		HasTransluscent = EFRIBool::False;
	}
};

struct Material
{
	typedef FStaticArray<MaterialMap, (size_t)EMaterialMap::MAX_MAPS> MapCollection;

	FRIContext* FriContext;
	MapCollection Maps;

	MaterialProperties Properties;

	Material() :
		FriContext(NULL)
	{
		Memory::Zero(Maps.Begin(), Maps.ByteSize());

	}

	Material(const MapCollection& maps, FRIContext* renderContext) : 
		Maps(maps), 
		FriContext(renderContext)
	{

	}

	void StageMemory(FRIMemoryMap& memory) const
	{
		memory << Properties;
	}

	inline static size_t GetStageMemorySize()
	{
		return sizeof(float) * 12;
	}


	void SetMap(EMaterialMap mapName, MaterialMap map)
	{
		Maps[(uint64)mapName] = map;
	}

	MaterialMap GetMap(EMaterialMap mapName)
	{
		return Maps[(uint64)mapName];
	}

	void SetWrapMode(EMaterialWrap wrap)
	{
		FRICommandList cmdList(FriContext->GetFRIDynamic());
		FArray<FTextureParameterBufferParameter> params;
		
		if (wrap == EMaterialWrap::Repeat)
		{
			params.Add(FTextureParameterBufferParameter(EFRITextureParamName::WrapS, EFRITextureWrapMode::Repeat));
			params.Add(FTextureParameterBufferParameter(EFRITextureParamName::WrapT, EFRITextureWrapMode::Repeat));
		}
		else if (wrap == EMaterialWrap::MirroredRepeat)
		{
			params.Add(FTextureParameterBufferParameter(EFRITextureParamName::WrapS, EFRITextureWrapMode::MirroredRepeat));
			params.Add(FTextureParameterBufferParameter(EFRITextureParamName::WrapT, EFRITextureWrapMode::MirroredRepeat));
		}
		else if (wrap == EMaterialWrap::MirroredClampEdge)
		{
			params.Add(FTextureParameterBufferParameter(EFRITextureParamName::WrapS, EFRITextureWrapMode::MirroredClampEdge));
			params.Add(FTextureParameterBufferParameter(EFRITextureParamName::WrapT, EFRITextureWrapMode::MirroredClampEdge));
		}


		for (int i = 0; i < (int)EMaterialMap::MAX_MAPS; i++)
		{
			if (Maps[i].Handle)
			{
				cmdList.SetTextureParameterBuffer(Maps[i].Handle, params);
			}
		}
	}

	void SetFilterMode(EMaterialFilter filter)
	{
		FRICommandList cmdList(FriContext->GetFRIDynamic());
		FArray<FTextureParameterBufferParameter> params;

		if (filter == EMaterialFilter::Nearest)
		{
			params.Add(FTextureParameterBufferParameter(EFRITextureParamName::MinFilter, EFRITextureFilterMode::Nearest));
			params.Add(FTextureParameterBufferParameter(EFRITextureParamName::MagFilter, EFRITextureFilterMode::Nearest));
		}
		else if (filter == EMaterialFilter::Bilinear)
		{
			params.Add(FTextureParameterBufferParameter(EFRITextureParamName::MinFilter, EFRITextureFilterMode::Bilinear));
			params.Add(FTextureParameterBufferParameter(EFRITextureParamName::MagFilter, EFRITextureFilterMode::Bilinear));
		}
		else if (filter == EMaterialFilter::Trilinear)
		{
			params.Add(FTextureParameterBufferParameter(EFRITextureParamName::MinFilter, EFRITextureFilterMode::Trilinear));
			params.Add(FTextureParameterBufferParameter(EFRITextureParamName::MagFilter, EFRITextureFilterMode::Trilinear));
		}


		for (int i = 0; i < (int)EMaterialMap::MAX_MAPS; i++)
		{
			if (Maps[i].Handle)
			{
				cmdList.SetTextureParameterBuffer(Maps[i].Handle, params);
			}
		}
	}
};