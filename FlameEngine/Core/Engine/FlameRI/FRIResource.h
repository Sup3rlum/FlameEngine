#pragma once


#include "../Renderer/Common/Color.h"
#include "Core/Math/Module.h"
#include "Core/Common/CoreCommon.h"
#include "FRIDefinitions.h"


typedef void* FRI_RESOURCE;

#if __cplusplus >= 202002L
template<class U, class T>
concept Derived = std::is_base_of<U, T>::value;
#endif

struct FRIResource
{

};

struct FRIResourceObject
{

	virtual FRIResource* GetResource() { return NULL; };
	virtual ~FRIResourceObject() {}
	virtual void Rename(const FString& Name) {};
};



struct FRIUnorderedAccessView : public FRIResourceObject
{
	FRIUnorderedAccessView()
	{

	}
};

struct FRIShaderResourceView : public FRIResourceObject
{
	FRIShaderResourceView()
	{

	}
};

struct FRIRenderTargetView : public FRIResourceObject
{
	FRIRenderTargetView()
	{

	}

	virtual class FRITexture2D* GetDepthBuffer() = 0;
};

struct FRIDepthStencilView : public FRIResourceObject
{
	FRIDepthStencilView()
	{

	}

};

struct FRIConstantBufferView : public FRIResourceObject
{
	FRIConstantBufferView()
	{

	}
};

struct FRIComputeResource
{
//	virtual FRIUnorderedAccessView* ComputeView() = 0;
};
struct FRITextureBase : FRIComputeResource
{
	//virtual FRIShaderResourceView* View() = 0;
};

struct FRITexture2D : public FRIResourceObject, FRITextureBase
{
	uint32 Width;
	uint32 Height;
	uint32 MipLevels;

	FRITexture2D(uint32 Width, uint32 Height, uint32 MipLevels) :
		Width(Width),
		Height(Height),
		MipLevels(MipLevels)
	{

	}

};
struct FRITexture3D : public FRIResourceObject, FRITextureBase
{
	uint32 Width;
	uint32 Height;
	uint32 Depth;
	uint32 MipLevels;

	FRITexture3D(uint32 Width, uint32 Height, uint32 Depth, uint32 MipLevels) :
		Width(Width),
		Height(Height),
		Depth(Depth),
		MipLevels(MipLevels)
	{

	}
};

struct FRITextureCubeMap : public FRIResourceObject, FRITextureBase
{
	uint32 Width;
	uint32 Height;
	uint32 MipLevels;

	FRITextureCubeMap(uint32 Width, uint32 Height, uint32 MipLevels) :
		Width(Width),
		Height(Height),
		MipLevels(MipLevels)
	{

	}
};
struct FRITexture2DArray : public FRIResourceObject, FRITextureBase
{
	uint32 Width;
	uint32 Height;
	uint32 NumLayers;
	uint32 MipLevels;

	FRITexture2DArray(uint32 Width, uint32 Height, uint32 NumLayers, uint32 MipLevels) :
		Width(Width),
		Height(Height),
		NumLayers(NumLayers),
		MipLevels(MipLevels)
	{

	}
};


/*  Buffers  */


struct FRIVertexBuffer : public FRIResourceObject
{
	uint32 VertexCount;
	uint32 VertexSize;
	EFRIUsage Usage;
	EFRIAccess Access;

	FRIVertexBuffer(uint32 VertexCount, uint32 VertexSize, EFRIUsage Usage, EFRIAccess Access) :
		VertexCount(VertexCount),
		VertexSize(VertexSize),
		Usage(Usage),
		Access(Access)
	{

	}
	uint32 GetByteSize() const
	{
		return VertexCount * VertexSize;
	}
};


struct FRIIndexBuffer : public FRIResourceObject
{
	uint32 IndexCount;
	EFRIUsage Usage;
	EFRIAccess Access;

	FRIIndexBuffer(uint32 IndexCount, EFRIUsage Usage, EFRIAccess Access) :
		IndexCount(IndexCount),
		Usage(Usage),
		Access(Access)
	{

	}
	uint32 GetByteSize() const
	{
		return IndexCount * sizeof(uint32);
	}
};

struct FRIInstanceBuffer : public FRIResourceObject
{
	uint32 InstanceCount;
	uint32 Stride;
	EFRIUsage Usage;
	EFRIAccess Access;

	FRIInstanceBuffer(uint32 InstanceCount, uint32 Stride, EFRIUsage Usage, EFRIAccess Access) : 
		InstanceCount(InstanceCount),
		Stride(Stride),
		Usage(Usage),
		Access(Access)
	{

	}

	uint32 GetByteSize() const
	{
		return InstanceCount * Stride;
	}
};

struct FRIConstantBuffer : public FRIResourceObject
{
	uint32 ByteSize;
	EFRIUsage Usage;
	EFRIAccess Access;

	FRIConstantBuffer(uint32 ByteSize, uint32 Stride, EFRIUsage Usage, EFRIAccess Access) :
		ByteSize(ByteSize),
		Usage(Usage),
		Access(Access)
	{
	}

	virtual FRIConstantBufferView* GetView()
	{
		return 0;
	}

};

struct FRIComputeBuffer : public FRIResourceObject, FRIComputeResource
{
	uint32 StructureCount;
	size_t StructureStride;
	EFRIUsage Usage;
	EFRIAccess Access;

	FRIComputeBuffer(uint32 Count, uint32 Stride, EFRIUsage Usage, EFRIAccess Access) :
		StructureCount(Count),
		StructureStride(Stride),
		Usage(Usage),
		Access(Access)
	{
	}
	uint32 GetByteSize() const
	{
		return StructureCount * StructureStride;
	}
};

struct FRISampler2
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


	FRISampler2(uint32 unit, FRITexture2D* val) : Unit(unit), samplerType(EFRISamplerType::TSampler2D), Param2D(val) {}
	FRISampler2(uint32 unit, FRITexture3D* val) : Unit(unit), samplerType(EFRISamplerType::TSampler3D), Param3D(val) {}
	FRISampler2(uint32 unit, FRITextureCubeMap* val) : Unit(unit), samplerType(EFRISamplerType::TSamplerCube), ParamCube(val) {}
	FRISampler2(uint32 unit, FRITexture2DArray* val) : Unit(unit), samplerType(EFRISamplerType::TSampler2DArray), Param2DArray(val) {}
	FRISampler2(uint32 unit) : Unit(unit), Param2D(NULL), samplerType(EFRISamplerType::MaxSamplerTypes) {  }

private:
	FRISampler2();
};

struct FRISamplerState : public FRIResourceObject
{
	FRISamplerState(

	)
	{}

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


struct FRIPipelineStateObject : FRIResourceObject
{

};

struct FRIRasterizerState : FRIResourceObject
{
	EFRICullMode CullMode;
	EFRIFillMode FillMode;

	FRIRasterizerState(EFRICullMode cullMode, EFRIFillMode fillMode) : CullMode(cullMode), FillMode(fillMode) {}
};

struct FRIBlendState : FRIResourceObject
{
	EFRIBlend SrcBlend;
	EFRIBlend DstBlend;

	FRIBlendState(EFRIBlend srcBlend, EFRIBlend dstBlend) : SrcBlend(srcBlend), DstBlend(dstBlend) {}
};

struct FRIDepthStencilState : FRIResourceObject
{
	EFRIBool EnableDepth;

	FRIDepthStencilState(EFRIBool enableDepth) : EnableDepth(enableDepth) {}
};


struct FRICreationDescriptor
{
public:
	FRICreationDescriptor() : FRICreationDescriptor(0, 0) {}
	explicit FRICreationDescriptor(size_t ByteSize) : FRICreationDescriptor(0, ByteSize) {}
	explicit FRICreationDescriptor(void* DataArray, size_t ByteSize) :
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
	FRIUpdateDescriptor(const void* DataArray, size_t position, size_t ByteSize, size_t RowPitch) :
		DataArray(DataArray),
		ByteSize(ByteSize),
		Position(position),
		RowPitch(RowPitch)
	{

	}
	FRIUpdateDescriptor(const void* DataArray, size_t position, size_t ByteSize) :
		DataArray(DataArray),
		ByteSize(ByteSize),
		Position(position),
		RowPitch(ByteSize)
	{

	}
	size_t Position;
	size_t ByteSize;
	size_t RowPitch;
	const void* DataArray;
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

/* Input Layout */

struct FRIInputElementDesc
{
	FRIInputSemantic Semantic;
	EFRIAttributeType Type;
	uint32 InputSlot;
	EFRIAttribUsage Usage;


	FRIInputElementDesc(FRIInputSemantic Semantic, EFRIAttributeType Type, uint32 InputSlot, EFRIAttribUsage Usage)
		: Semantic(Semantic),
		Type(Type),
		InputSlot(InputSlot),
		Usage(Usage)
	{}
};

struct FRIInputLayout 
{
	uint32 NumElements;
	FRIInputElementDesc* DeclarationElements;
};

/* Render Targets */


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


typedef byte FRIByte;


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

	template<typename ...Types>
	void LoadMulti(const Types& ... args)
	{
		(*this << ... << args);
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
		MemoryPtr = (byte*)_aligned_malloc(8192, 16);
	}

};


struct FRIStageBuffer
{
	FRIConstantBuffer* GPU;
	FRIMemoryMap Stage;
};

typedef FDelegate<void(FRIMemoryMap&)> FRIMemoryStageDelegate;


typedef FRefPtr<FRITexture2D> FRITexture2DRef;
typedef FRefPtr<FRITexture2DArray> FRITexture2DArrayRef;
typedef FRefPtr<FRIRasterizerState> FRIRasterizerStateRef;
typedef FRefPtr<FRIDepthStencilState> FRIDepthStencilStateRef;
typedef FRefPtr<FRIBlendState> FRIBlendStateRef;
typedef FRefPtr<FRITextureCubeMap> FRITextureCubeMapRef;
		
typedef FRefPtr<FRIRenderTargetView> FRIRenderTargetViewRef;
typedef FRefPtr<FRIVertexBuffer> FRIVertexBufferRef;
typedef FRefPtr<FRIIndexBuffer> FRIIndexBufferRef;
typedef FRefPtr<FRIInstanceBuffer> FRIInstanceBufferRef;
typedef FRefPtr<FRIConstantBuffer> FRIConstantBufferRef;
typedef FRefPtr<FRIInputLayout> FRIInputLayoutRef;
typedef FRefPtr<FRIShaderPipeline> FRIShaderPipelineRef;

/*
template<size_t ResourceSize>
struct RenderStruct
{
	inline static size_t GetStageMemorySize()
	{
		return sizeof(float) * ResourceSize;
	}
};*/

template<typename...TArgs>
struct RenderStruct
{
	inline static size_t GetStageMemorySize()
	{
		return SumSizes<TArgs...>();
	}
};

/*
#define Float4 FVector4
#define Int4 IVector4
#define Float3 FVector3
#define Int3 IVector3
#define Float2 FVector2
#define Int2 IVector2

#define Float float
#define Int int
#define Matrix FMatrix4*/