#pragma once

#include "../../FlameRI/FRI.h"
#include "../EntityComponent/EntityComponent.h"
#include "MaterialDefinitions.h"

#include "../../PCI/PCI.h"


enum class EMaterialLayer : uint32
{
	Diffuse = 0,
	Normal = 1,
	Detail = 2,
	Emissive = 3,
	NUM_LAYERS = 4
};


struct MaterialLayer
{
	FRITexture2D* Handle;
	FRIShaderResourceView* View;
	FRISamplerState* Sampler;

	MaterialLayer(FRITexture2D* Handle, FRIShaderResourceView* View, FRISamplerState* Sampler) : 
		Handle(Handle), 
		View(View),
		Sampler(Sampler)
	{}
	MaterialLayer() : 
		Handle(NULL), 
		View(NULL),
		Sampler(NULL)
	{}
};

class Material : IProperties, RenderStruct<FVector4, FVector4, FVector4>
{
public:
	typedef FStaticArray<MaterialLayer, (uint64)EMaterialLayer::NUM_LAYERS> LayerArray;
	LayerArray Layers;

	PropertyEnum(EFRIBool, UsesNormalMap, EFRIBool::True)
	PropertyEnum(EFRIBool, UsesPOM, EFRIBool::True)
	PropertyEnum(EFRIBool, UsesAOMap, EFRIBool::True)
	PropertyEnum(EFRIBool, UsesEmissiveColor, EFRIBool::False)
	PropertyEnum(EFRIBool, UsesTransluscency, EFRIBool::False)

	Material()
	{
		Memory::Zero(Layers.Begin(), Layers.ByteSize());
	}

	Material(const LayerArray& layers) :
		Layers(layers)
	{
	}

	void StageMemory(FRIMemoryMap& GPUMemory) const
	{
		GPUMemory.LoadMulti(UsesNormalMap, UsesPOM, UsesAOMap, UsesEmissiveColor, UsesTransluscency);
	}

	FRIShaderResourceView*			GetLayerView(EMaterialLayer mapName)		{ return Layers[(uint64)mapName].View; }
	const FRIShaderResourceView*	GetLayerView(EMaterialLayer mapName) const	{ return Layers[(uint64)mapName].View; }

	MaterialLayer&			GetLayer(EMaterialLayer mapName)		{ return Layers[(uint64)mapName]; }
	const MaterialLayer&	GetLayer(EMaterialLayer mapName) const	{ return Layers[(uint64)mapName]; }
};

