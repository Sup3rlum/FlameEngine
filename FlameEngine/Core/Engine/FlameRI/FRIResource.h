#pragma once

#include "FRI.h"
#include "FRIDefinitions.h"
#include "../Renderer/Common/Color.h"
#include "Core/Math/Module.h"


struct FResourceObject
{

};


struct FResourceTexture2D : public FResourceObject
{
	uint32 Width;
	uint32 Height;
	uint32 SampleCount;

	FResourceTexture2D(uint32 Width, uint32 Height, uint32 SampleCount) :
		Width(Width),
		Height(Height),
		SampleCount(SampleCount)
	{

	}
};
struct FResourceTexture3D : public FResourceObject
{
	uint32 Width;
	uint32 Height;
	uint32 Depth;
	uint32 SampleCount;

	FResourceTexture3D(uint32 Width, uint32 Height, uint32 Depth, uint32 SampleCount) :
		Width(Width),
		Height(Height),
		Depth(Depth),
		SampleCount(SampleCount)
	{

	}
};

struct FResourceTextureCubeMap : public FResourceObject
{
	uint32 Width;
	uint32 Height;
	uint32 SampleCount;

	FResourceTextureCubeMap(uint32 Width, uint32 Height, uint32 SampleCount) :
		Width(Width),
		Height(Height),
		SampleCount(SampleCount)
	{

	}
};




struct FResourceVertexBuffer : public FResourceObject
{
	uint32 Size;
	uint32 Usage;

	FResourceVertexBuffer(uint32 Size, uint32 Usage) :
		Size(Size),
		Usage(Usage)
	{

	}

};


struct FResourceIndexBuffer : public FResourceObject
{
	uint32 Size;
	uint32 Usage;

	FResourceIndexBuffer(uint32 Size, uint32 Usage) :
		Size(Size),
		Usage(Usage)
	{

	}

};



struct FResourceFrameBuffer : public FResourceObject
{
	uint32 Size;
	uint32 Usage;

	FResourceFrameBuffer(uint32 Size, uint32 Usage) :
		Size(Size),
		Usage(Usage)
	{

	}

};



struct FResourceUniformBuffer : public FResourceObject
{
	struct FUniformParameter
	{
		uint32 Location;
		EFRIUniformBufferParameterType paramType;
	};

	FArray<FUniformParameter> Data;


	FResourceUniformBuffer(FArray<FUniformParameter> Data)
	{
		this->Data = Data;
	}

};

struct FResourceTextureParameterBuffer : public FResourceObject
{
	struct FTextureParameterBufferParameter
	{
		uint32 Parameter;
		EFRIUniformBufferParameterType paramType;
	};

	FArray<FTextureParameterBufferParameter> Data;

	FResourceTextureParameterBuffer(FArray<FTextureParameterBufferParameter> Data)
	{
		this->Data = Data;
	}


};


/*****************************
* 
*			SHADERS
* 
******************************/

struct FResourceShaderBase : public FResourceObject
{

	FString Name;

	FResourceShaderBase(FString Name) :
		Name(Name)
	{

	}
};


struct FResourceVertexShader : public FResourceObject {};
struct FResourcePixelShader : public FResourceObject {};
struct FResourceGeometryShader : public FResourceObject {};
struct FResourceHullShader : public FResourceObject {};
struct FResourceDomainShader : public FResourceObject {};
struct FResourceComputeShader : public FResourceObject {};


struct FResourceArrayInterface
{
	virtual void* GetResourceData() = NULL;
	virtual size_t GetResourceDataSize() = 0;
};



struct FResourceCreationDescriptor
{
	FResourceCreationDescriptor(FResourceArrayInterface* DataArray) :
		DataArray(DataArray)
	{

	}


	FResourceArrayInterface* DataArray;
};