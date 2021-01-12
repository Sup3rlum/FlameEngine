#pragma once


#include "FRIDefinitions.h"
#include "../Renderer/Common/Color.h"
#include "Core/Math/Module.h"
#include "Core/Common/CoreCommon.h"


struct FResourceObject
{
	virtual bool IsValidResource() { return false; };
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

	FResourceTexture3D(uint32 Width, uint32 Height, uint32 Depth) :
		Width(Width),
		Height(Height),
		Depth(Depth)
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
	uint32 IndexCount;
	uint32 Usage;

	FResourceIndexBuffer(uint32 IndexCount, uint32 Usage) :
		IndexCount(IndexCount),
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

		uint32 Param;

		union
		{
			int32 EnumParam;
			float FloatParam;
		};

		EFRITextureParameterBufferParameterType paramType;
	};

	FArray<FTextureParameterBufferParameter> Data;

	FResourceTextureParameterBuffer(FArray<FTextureParameterBufferParameter> Data)
	{
		this->Data = Data;
	}


};


struct FResourceRenderQuery : public FResourceObject
{
	FResourceRenderQuery()
	{

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

	virtual uint32 GetResource() const = 0;

};


struct FResourceVertexShader : public FResourceShaderBase { FResourceVertexShader(FString Name) : FResourceShaderBase(Name) {} };
struct FResourcePixelShader : public FResourceShaderBase { FResourcePixelShader(FString Name) : FResourceShaderBase(Name) {} };
struct FResourceGeometryShader : public FResourceShaderBase { FResourceGeometryShader(FString Name) : FResourceShaderBase(Name) {} };
struct FResourceHullShader : public FResourceShaderBase { FResourceHullShader(FString Name) : FResourceShaderBase(Name) {} };
struct FResourceDomainShader : public FResourceShaderBase { FResourceDomainShader(FString Name) : FResourceShaderBase(Name) {} };
struct FResourceComputeShader : public FResourceShaderBase { FResourceComputeShader(FString Name) : FResourceShaderBase(Name) {} };

struct FResourceShaderPipelineCreationDescriptor
{
	uint32 NumShaders;
	FResourceShaderBase* ShaderArray;

	FResourceShaderPipelineCreationDescriptor(uint32 NumShaders, FResourceShaderBase* ShaderArray) :
		NumShaders(NumShaders),
		ShaderArray(ShaderArray)
	{

	}
};


struct FResourceShaderPipeline : FResourceObject
{
	FResourceShaderPipeline(FResourceShaderPipelineCreationDescriptor descriptor)
	{

	}
};


struct FResourceArrayInterface
{
	virtual void* GetResourceData() = 0;
	virtual size_t GetResourceDataSize() = 0;
};



struct FResourceCreationDescriptor
{
public:
	FResourceCreationDescriptor(FResourceArrayInterface* DataArray, size_t ByteSize) :
		DataArray(DataArray),
		ByteSize(ByteSize)
	{

	}
	size_t ByteSize;
	FResourceArrayInterface* DataArray;
};

struct FResourceVertexDeclarationComponent
{
	uint32 AttribNumber;
	uint32 Type;
	bool Normalized;
	uint32 Stride;
	uint32 ByteSize;
	void* Offset;

	FResourceVertexDeclarationComponent(uint32 attribNumber, uint32 byteSize, uint32 type, bool norm, uint32 stride, void* offset) :
		AttribNumber(attribNumber),
		Type(type),
		Normalized(norm),
		Stride(stride),
		Offset(offset),
		ByteSize(byteSize)
	{
	}
};


struct FResourceVertexDeclaration : FResourceObject
{
	FArray<FResourceVertexDeclarationComponent> DeclarationElements;

	FResourceVertexDeclaration(const FArray<FResourceVertexDeclarationComponent>& decl) :
		DeclarationElements(decl)
	{
	}
};