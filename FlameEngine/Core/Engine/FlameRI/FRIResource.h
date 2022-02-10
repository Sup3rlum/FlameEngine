#pragma once


#include "FRIDefinitions.h"
#include "../Renderer/Common/Color.h"
#include "Core/Math/Module.h"
#include "Core/Common/CoreCommon.h"


struct FRIResourceObject
{
	virtual bool IsValidResource() { return false; };

	virtual ~FRIResourceObject() {}
};


struct FRITexture2D : public FRIResourceObject
{
	uint32 Width;
	uint32 Height;
	uint32 SampleCount;

	FRITexture2D(uint32 Width, uint32 Height, uint32 SampleCount) :
		Width(Width),
		Height(Height),
		SampleCount(SampleCount)
	{

	}
};
struct FRITexture3D : public FRIResourceObject
{
	uint32 Width;
	uint32 Height;
	uint32 Depth;

	FRITexture3D(uint32 Width, uint32 Height, uint32 Depth) :
		Width(Width),
		Height(Height),
		Depth(Depth)
	{

	}
};

struct FRITextureCubeMap : public FRIResourceObject
{
	uint32 Width;
	uint32 Height;
	uint32 SampleCount;

	FRITextureCubeMap(uint32 Width, uint32 Height, uint32 SampleCount) :
		Width(Width),
		Height(Height),
		SampleCount(SampleCount)
	{

	}
};
struct FRITexture2DArray : public FRIResourceObject
{
	uint32 Width;
	uint32 Height;
	uint32 NumLayers;

	FRITexture2DArray(uint32 Width, uint32 Height, uint32 NumLayers) :
		Width(Width),
		Height(Height),
		NumLayers(NumLayers)
	{

	}
};




struct FRIVertexBuffer : public FRIResourceObject
{
	uint32 Size;
	uint32 Usage;

	FRIVertexBuffer(uint32 Size, uint32 Usage) :
		Size(Size),
		Usage(Usage)
	{

	}

};


struct FRIIndexBuffer : public FRIResourceObject
{
	uint32 IndexCount;
	uint32 Usage;

	FRIIndexBuffer(uint32 IndexCount, uint32 Usage) :
		IndexCount(IndexCount),
		Usage(Usage)
	{

	}

};

struct FRIInstanceBuffer : public FRIResourceObject
{
	uint32 Size;

	FRIInstanceBuffer(uint32 Size) : 
		Size(Size)
	{

	}
};



struct FRIFrameBuffer : public FRIResourceObject
{
	uint32 Size;
	uint32 Usage;

	FRIFrameBuffer(uint32 Size, uint32 Usage) :
		Size(Size),
		Usage(Usage)
	{

	}

	virtual FRITexture2D* GetDepthBuffer() = 0;

};



struct FUniformSampler
{
	uint32 Unit;
	EFRIUniformSamplerType samplerType;

	union
	{
		FRITexture2D* Param2D;
		FRITexture3D* Param3D;
		FRITextureCubeMap* ParamCube;
		FRITexture2DArray* Param2DArray;
	};


	FUniformSampler(uint32 unit, FRITexture2D* val) : Unit(unit), samplerType(EFRIUniformSamplerType::TSampler2D), Param2D(val) {}
	FUniformSampler(uint32 unit, FRITexture3D* val) : Unit(unit), samplerType(EFRIUniformSamplerType::TSampler3D), Param3D(val) {}
	FUniformSampler(uint32 unit, FRITextureCubeMap* val) : Unit(unit), samplerType(EFRIUniformSamplerType::TSamplerCube), ParamCube(val) {}
	FUniformSampler(uint32 unit, FRITexture2DArray* val) : Unit(unit), samplerType(EFRIUniformSamplerType::TSampler2DArray), Param2DArray(val) {}
	FUniformSampler(uint32 unit) : Unit(unit), Param2D(NULL) {  }

private:
	FUniformSampler();
};



struct FRIUniformBuffer : public FRIResourceObject
{
	size_t ByteSize;

	FRIUniformBuffer(size_t ByteSize) :
		ByteSize(ByteSize)
	{
	}
};

struct FTextureParameterBufferParameter
{
	uint32 ParamName;
	union
	{
		int32 EnumParam;
		float FloatParam;
	};
	EFRITextureParameterBufferParameterType paramType;

	FTextureParameterBufferParameter(EFRITextureParamName ParamName, EFRITextureWrapMode mode)		: ParamName((uint32)ParamName), paramType(EFRITextureParameterBufferParameterType::Enum), EnumParam((int32)mode) {}
	FTextureParameterBufferParameter(EFRITextureParamName ParamName, EFRITextureFilterMode mode)	: ParamName((uint32)ParamName), paramType(EFRITextureParameterBufferParameterType::Enum), EnumParam((int32)mode) {}
	FTextureParameterBufferParameter(EFRITextureParamName ParamName, float value)					: ParamName((uint32)ParamName), paramType(EFRITextureParameterBufferParameterType::Float), FloatParam(value) {}

private:
	FTextureParameterBufferParameter();
};


struct FRITextureParameterBuffer : public FRIResourceObject
{


	FArray<FTextureParameterBufferParameter> Data;

	FRITextureParameterBuffer(const FArray<FTextureParameterBufferParameter>& Data) :
		Data(Data)
	{

	}
	FRITextureParameterBuffer(const FRITextureParameterBuffer& other) :
		Data(other.Data)
	{

	}
};


struct FRIRenderQuery : public FRIResourceObject
{
	FRIRenderQuery()
	{

	}
};




/*****************************
* 
*			SHADERS
* 
******************************/

struct FRIShaderBase : public FRIResourceObject
{
	EFRIResourceShaderType Type;

	FRIShaderBase(EFRIResourceShaderType type) :
		Type(type)
	{

	}

	virtual uint32 GetResource() const = 0;

};


struct FRIVertexShader	: public FRIShaderBase { FRIVertexShader()		: FRIShaderBase(EFRIResourceShaderType::Vertex) {} };
struct FRIPixelShader		: public FRIShaderBase { FRIPixelShader()		: FRIShaderBase(EFRIResourceShaderType::Pixel) {} };
struct FRIGeometryShader	: public FRIShaderBase { FRIGeometryShader()	: FRIShaderBase(EFRIResourceShaderType::Geometry) {} };
struct FRIHullShader		: public FRIShaderBase { FRIHullShader()		: FRIShaderBase(EFRIResourceShaderType::Hull) {} };
struct FRIDomainShader	: public FRIShaderBase { FRIDomainShader()		: FRIShaderBase(EFRIResourceShaderType::Domain) {} };
struct FRIComputeShader	: public FRIShaderBase { FRIComputeShader()		: FRIShaderBase(EFRIResourceShaderType::Compute) {} };

struct FRIShaderPipelineCreationDescriptor
{
	uint32 NumShaders;
	FRIShaderBase** ShaderArray;

	FRIShaderPipelineCreationDescriptor(uint32 NumShaders, FRIShaderBase** ShaderArray) :
		NumShaders(NumShaders),
		ShaderArray(ShaderArray)
	{

	}
};


struct FRIShaderPipeline : FRIResourceObject
{
	FRIShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor)
	{

	}
};

struct FRIRasterizerState
{
	
};

struct FRIBlendState
{

};

struct FRIDepthStencilState
{

};


struct FRIArrayInterface
{

};



struct FRICreationDescriptor
{
public:
	FRICreationDescriptor(void* DataArray, size_t ByteSize) :
		DataArray(DataArray),
		ByteSize(ByteSize)
	{

	}
	size_t ByteSize;
	//FRIArrayInterface* DataArray;
	void* DataArray;
};



struct FRIUpdateDescriptor
{
public:
	FRIUpdateDescriptor(void* DataArray, size_t position, size_t ByteSize) :
		DataArray(DataArray),
		ByteSize(ByteSize),
		Position(position)
	{

	}
	size_t Position;
	size_t ByteSize;
	//FRIArrayInterface* DataArray;
	void* DataArray;
};


struct FRIInputSemantic
{
	const char* SemanticName;
	uint32 SemanticIndex;

	FRIInputSemantic(const char* name) :
		SemanticName(name),
		SemanticIndex(0)
	{}

	FRIInputSemantic(const char* name, uint32 index) :
		SemanticName(name),
		SemanticIndex(index)
	{}

	FRIInputSemantic(const FRIInputSemantic& other) :
		SemanticName(other.SemanticName),
		SemanticIndex(other.SemanticIndex)
	{

	}

};




struct FRIVertexDeclarationComponent
{
	FRIInputSemantic Semantic;
	uint32 Length;
	EFRIVertexDeclerationAttributeType Type;
	EFRIBool Normalized;
	uint32 Stride;
	uint32 Offset;
	EFRIAttribUsage Usage;

	FRIVertexDeclarationComponent(FRIInputSemantic Semantic, uint32 length, EFRIVertexDeclerationAttributeType type, EFRIBool norm, uint32 stride, uint32 offset, EFRIAttribUsage Usage = EFRIAttribUsage::PerVertex) :
		Semantic(Semantic),
		Type(type),
		Normalized(norm),
		Stride(stride),
		Offset(offset),
		Length(length),
		Usage(Usage)
	{
	}

	FRIVertexDeclarationComponent(const FRIVertexDeclarationComponent& other) :
		Semantic(other.Semantic),
		Type(other.Type),
		Normalized(other.Normalized),
		Stride(other.Stride),
		Offset(other.Offset),
		Length(other.Length),
		Usage(other.Usage)
	{

	}

};

struct FRIVertexDeclarationDesc
{
	FArray<FRIVertexDeclarationComponent> Components;
	uint32 InputSlot;

	FRIVertexDeclarationDesc() :
		InputSlot(0)
	{

	}

	FRIVertexDeclarationDesc(const FArray<FRIVertexDeclarationComponent>& Components, uint32 InputSlot) :
		Components(Components),
		InputSlot(InputSlot)
	{

	}

	FRIVertexDeclarationDesc(const FRIVertexDeclarationDesc& other) :
		Components(other.Components),
		InputSlot(other.InputSlot)
	{

	}

};


struct FRIVertexDeclaration
{
	FArray<FRIVertexDeclarationDesc> DeclarationElements;

	FRIVertexDeclaration(const FArray<FRIVertexDeclarationDesc>& decl) :
		DeclarationElements(decl)
	{
	}
};

struct FRIFrameBufferAttachment
{

	FRITexture2D* Param2D;

	FRIFrameBufferAttachment(FRITexture2D* texture) : 
		Param2D(texture) 
	{
	}
};


struct FRIFrameBufferArrayAttachment
{
	FRITexture2DArray* Param2DArray;

	FRIFrameBufferArrayAttachment(FRITexture2DArray* texture) : Param2DArray(texture) {}
};





struct FRIColorDataFormat
{
	EFRIChannels channelFormat;
	EFRIPixelStorage pixelStorage;



	FRIColorDataFormat(EFRIChannels channelFormat, EFRIPixelStorage pixelStorage) :
		channelFormat(channelFormat),
		pixelStorage(pixelStorage)
	{

	}


};



struct FRIByte : FRIArrayInterface
{
	byte _Internal;
};



struct FRIMemoryMap
{
	size_t Head = 0;
	byte* MemoryPtr;

	void Load(const void* src, size_t byteSize)
	{
		Memory::Copy(MemoryPtr + Head, (byte*)src, byteSize);
		Head += byteSize;
	}

	template<typename Type>
	void Load(const Type& value)
	{
		Load((const void*)&value, sizeof(Type));
	}



};


struct IFRIStageableResource
{
	virtual void StageMemory(FRIMemoryMap& stagememory) const = 0;
	virtual size_t GetStageMemorySize() const = 0;
};


typedef FDelegate<void(FRIMemoryMap&)> FRIMemoryStageDelegate;