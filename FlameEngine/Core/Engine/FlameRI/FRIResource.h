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

struct FRIBuffer
{

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
	uint32 MipLevels;

	FRITextureCubeMap(uint32 Width, uint32 Height, uint32 SampleCount) :
		Width(Width),
		Height(Height),
		MipLevels(SampleCount)
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


/*  Buffers  */


struct FRIVertexBuffer : public FRIResourceObject
{
	uint32 ByteSize;
	uint32 Usage;
	EFRIAccess Access;

	FRIVertexBuffer(uint32 ByteSize, uint32 Usage, EFRIAccess Access) :
		ByteSize(ByteSize),
		Usage(Usage),
		Access(Access)
	{

	}

};


struct FRIIndexBuffer : public FRIResourceObject
{
	uint32 IndexCount;
	uint32 Usage;
	EFRIAccess Access;

	FRIIndexBuffer(uint32 IndexCount, uint32 Usage, EFRIAccess Access) :
		IndexCount(IndexCount),
		Usage(Usage),
		Access(Access)
	{

	}

};

struct FRIInstanceBuffer : public FRIResourceObject
{
	uint32 Stride;
	uint32 ByteSize;
	uint32 Usage;
	EFRIAccess Access;

	FRIInstanceBuffer(uint32 Size, uint32 Stride, uint32 Usage, EFRIAccess Access) : 
		Stride(Stride),
		ByteSize(ByteSize),
		Usage(Usage),
		Access(Access)
	{

	}
};

struct FRIUniformBuffer : public FRIResourceObject
{
	uint32 ByteSize;
	uint32 Usage;
	EFRIAccess Access;

	FRIUniformBuffer(uint32 Size, uint32 Stride, uint32 Usage, EFRIAccess Access) :
		ByteSize(ByteSize),
		Usage(Usage),
		Access(Access)
	{
	}
};

struct FRIComputeBuffer : public FRIResourceObject
{
	size_t StructureStride;
	uint32 ByteSize;
	uint32 Usage;
	EFRIAccess Access;

	FRIComputeBuffer(uint32 Size, uint32 Stride, uint32 Usage, EFRIAccess Access) :
		StructureStride(StructureStride),
		ByteSize(ByteSize),
		Usage(Usage),
		Access(Access)
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



struct FRISampler
{
	uint32 Unit;
	EFRISamplerType samplerType;

	union
	{
		FRITexture2D* Param2D;
		FRITexture3D* Param3D;
		FRITextureCubeMap* ParamCube;
		FRITexture2DArray* Param2DArray;
	};


	FRISampler(uint32 unit, FRITexture2D* val) : Unit(unit), samplerType(EFRISamplerType::TSampler2D), Param2D(val) {}
	FRISampler(uint32 unit, FRITexture3D* val) : Unit(unit), samplerType(EFRISamplerType::TSampler3D), Param3D(val) {}
	FRISampler(uint32 unit, FRITextureCubeMap* val) : Unit(unit), samplerType(EFRISamplerType::TSamplerCube), ParamCube(val) {}
	FRISampler(uint32 unit, FRITexture2DArray* val) : Unit(unit), samplerType(EFRISamplerType::TSampler2DArray), Param2DArray(val) {}
	FRISampler(uint32 unit) : Unit(unit), Param2D(NULL), samplerType(EFRISamplerType::MaxSamplerTypes) {  }

private:
	FRISampler();
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
	EFRIShaderType Type;

	FRIShaderBase(EFRIShaderType type) :
		Type(type)
	{

	}

	virtual uint32 GetResource() const = 0;

};


struct FRIVertexShader	: public FRIShaderBase { FRIVertexShader()		: FRIShaderBase(EFRIShaderType::Vertex) {} };
struct FRIPixelShader		: public FRIShaderBase { FRIPixelShader()		: FRIShaderBase(EFRIShaderType::Pixel) {} };
struct FRIGeometryShader	: public FRIShaderBase { FRIGeometryShader()	: FRIShaderBase(EFRIShaderType::Geometry) {} };
struct FRIHullShader		: public FRIShaderBase { FRIHullShader()		: FRIShaderBase(EFRIShaderType::Hull) {} };
struct FRIDomainShader	: public FRIShaderBase { FRIDomainShader()		: FRIShaderBase(EFRIShaderType::Domain) {} };
struct FRIComputeShader	: public FRIShaderBase { FRIComputeShader()		: FRIShaderBase(EFRIShaderType::Compute) {} };

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
	FRICreationDescriptor() : FRICreationDescriptor(0, 0) {}
	FRICreationDescriptor(size_t ByteSize) : FRICreationDescriptor(0, ByteSize) {}
	FRICreationDescriptor(void* DataArray, size_t ByteSize) :
		DataArray(DataArray),
		ByteSize(ByteSize)
	{

	}

	size_t ByteSize;
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




struct FRIInputAttribute
{
	FRIInputSemantic Semantic;
	uint32 Length;
	EFRIAttributeType Type;
	EFRIBool Normalized;
	uint32 Stride;
	uint32 Offset;
	EFRIAttribUsage Usage;

	FRIInputAttribute(FRIInputSemantic Semantic, uint32 length, EFRIAttributeType type, EFRIBool norm, uint32 stride, uint32 offset, EFRIAttribUsage Usage = EFRIAttribUsage::PerVertex) :
		Semantic(Semantic),
		Type(type),
		Normalized(norm),
		Stride(stride),
		Offset(offset),
		Length(length),
		Usage(Usage)
	{
	}

	FRIInputAttribute(const FRIInputAttribute& other) :
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


typedef FArray<FRIInputAttribute> InputAttributeArray;

struct FRIInputDesc
{
	FArray<FRIInputAttribute> Components;
	uint32 InputSlot;

	FRIInputDesc() :
		InputSlot(0)
	{

	}

	FRIInputDesc(const FArray<FRIInputAttribute>& Components, uint32 InputSlot) :
		Components(Components),
		InputSlot(InputSlot)
	{

	}

	FRIInputDesc(const FRIInputDesc& other) :
		Components(other.Components),
		InputSlot(other.InputSlot)
	{

	}

};


struct FRIVertexDeclaration
{
	FArray<FRIInputDesc> DeclarationElements;

	FRIVertexDeclaration(const FArray<FRIInputDesc>& decl) :
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
	uint32 IsCube;

	union
	{
		FRITexture2DArray* Param2DArray;
		FRITextureCubeMap* ParamCube;
	};

	FRIFrameBufferArrayAttachment(FRITexture2DArray* texture) : Param2DArray(texture), IsCube(0) {}
	FRIFrameBufferArrayAttachment(FRITextureCubeMap* texture) : ParamCube(texture), IsCube(1) {}
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

	template<typename Type>
	FRIMemoryMap& operator<<(const Type& type)
	{
		Load(type);

		return *this;
	}

	~FRIMemoryMap()
	{
		_aligned_free(MemoryPtr);
	}

	template<typename TLambda>
	void Begin(TLambda&& lambda)
	{
		Head = 0;

		lambda(*this);
	}

	FRIMemoryMap()
	{
		MemoryPtr = (byte*)_aligned_malloc(4096, 16);
	}

};


struct FRIStageBuffer
{
	FRIUniformBuffer* GPU;
	FRIMemoryMap Stage;
};

typedef FDelegate<void(FRIMemoryMap&)> FRIMemoryStageDelegate;





typedef FRefPtr<FRITexture2D> FRITexture2DRef;
typedef FRefPtr<FRITexture2DArray> FRITexture2DArrayRef;
typedef FRefPtr<FRIRasterizerState> FRIRasterizerStateRef;
typedef FRefPtr<FRIDepthStencilState> FRIDepthStencilStateRef;
typedef FRefPtr<FRIBlendState> FRIBlendStateRef;
typedef FRefPtr<FRITextureCubeMap> FRITextureCubeMapRef;
		
typedef FRefPtr<FRIFrameBuffer> FRIFrameBufferRef;
typedef FRefPtr<FRIVertexBuffer> FRIVertexBufferRef;
typedef FRefPtr<FRIIndexBuffer> FRIIndexBufferRef;
typedef FRefPtr<FRIInstanceBuffer> FRIInstanceBufferRef;
typedef FRefPtr<FRIUniformBuffer> FRIUniformBufferRef;
typedef FRefPtr<FRIVertexDeclaration> FRIVertexDeclarationRef;
typedef FRefPtr<FRIShaderPipeline> FRIShaderPipelineRef;
