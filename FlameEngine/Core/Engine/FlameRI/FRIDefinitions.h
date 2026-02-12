#pragma once


typedef unsigned int uint32;


enum class EFRISamplerType
{
	TSampler2D,
	TSampler3D,
	TSamplerCube,
	TSampler2DArray,
	MaxSamplerTypes
};


enum class EFRITextureParameterBufferParameterType
{
	Enum,
	Float
};

enum class EFRIShaderType 
{
	Vertex,
	Pixel,
	Geometry,
	Hull,
	Domain,
	Compute
};

enum class EFRIAttributeType : unsigned int
{
	Float,
	Float2,
	Float3,
	Float4,
	Int,
	Int2,
	Int3,
	Int4
};

enum class EFRIPrimitiveType
{
	Points,
	Lines = 0x0001,
	Triangles = 0x0004,
	ControlPoint3 = 35
};

enum class EFRIIndexType : unsigned int
{
	UInt8 = 0x1401,
	UInt16 = 0x1403,
	UInt32 = 0x1405
};

enum class EFRIBool : unsigned int
{
	False = 0,
	True = 1
};

struct FRIBool
{
private:
	uint32 Value;
public:
	FRIBool() : Value(0) {}
	FRIBool(const FRIBool& other) : Value(other.Value) {}
	FRIBool(const EFRIBool& other) : Value((uint32)other) {}
	FRIBool(uint32 Value) : 
		Value(Value)
	{}


	operator bool()
	{
		return (bool)Value;
	}

};

enum class EFRITextureAddress
{
	Repeat,
	ClampEdge,
	ClampBorder,
	MirroredRepeat,
	MirroredClampEdge
};

enum class EFRITextureFilter
{
	Point,
	Bilinear,
	Trilinear,
	Anisotropic4,
	Anisotropic8
};

enum class EFRIRendererFramework
{
	None,
	OpenGL,
	DX11,
	DX12,
	Vulkan 
};

enum class EResourceFBTextureAttachmentType
{
	Color,
	Depth
};



enum class EFRITextureFormat
{
	R16F,
	RG16F,
	RGB16F,
	RGBA16F,

	R32F,
	RG32F,
	RGB32F,
	RGBA32F,


	R32UI,
	RG32UI,
	RGB32UI,
	RGBA32UI,

	R16UI,
	RG16UI,
	RGB16UI,
	RGBA16UI,

	R8UI,
	RG8UI,
	RGB8UI,
	RGBA8UI,


	R8UNORM,
	RG8UNORM,
	RGB8UNORM,
	RGBA8UNORM,

	R16UNORM,
	RG16UNORM,
	RGB16UNORM,
	RGBA16UNORM,

	DEPTH32

};

enum class EFRIChannels
{
	R,
	RG,
	RGB,
	RGBA
};

enum class EFRIPixelStorage
{
	Float,
	Half,
	Int,
	Short,
	Byte,
	UnsignedInt,
	UnsignedShort,
	UnsignedByte
};

enum class EFRIFillMode
{
	Solid,
	Wireframe
};

enum class EFRICullMode
{
	None,
	Front,
	Back
};

enum class EFRIBlend
{
	Zero,
	One,
	Src,
	OneMinusSrc,
	Dst,
	OneMinusDst
};


enum class EFRIAttribUsage
{
	PerVertex,
	PerInstance
};

enum class EFRIAccess
{
	None,
	Read,
	Write,
	ReadWrite
};

enum class EFRIUsage
{
	Default,
	Dynamic,
	Staging,
	Immutable
};

enum EFRIPipelineStage
{
	EFRI_Vertex = 1,
	EFRI_Pixel = 2,
	EFRI_Geometry = 4,
	EFRI_Domain = 8,
	EFRI_Hull = 16,
	EFRI_Compute = 32
};



enum class EFRIShaderVisibility
{
	All = 0,
	Vertex = 1,
	Hull = 2,
	Domain = 3,
	Geometry = 4,
	Pixel = 5,
	Amplification = 6,
	Mesh = 7
};

enum class EFRIRootParameterType
{
	DESCRIPTOR_TABLE = 0,
	CONSTANTS = 1,
	CBV = 2,
	SRV = 3,
	UAV = 4
};

enum class EState : uint32
{
	Common = 0,
	VertexAndConstantBuffer = 0x1,
	IndexBuffer = 0x2,
	RenderTarget = 0x4,
	UnorderedAccess = 0x8,
	DepthWrite = 0x10,
	DepthRead = 0x20,
	NonPixelShaderResource = 0x40,
	PixelShaderResource = 0x80,
	StreamOut = 0x100,
	IndirectArgument = 0x200,
	CopyDest = 0x400,
	CopySource = 0x800,
	ResolveDest = 0x1000,
	ResolveSource = 0x2000,
	RaytracingAccelerationStructure = 0x400000,
	ShadingRateSource = 0x1000000,
	GenericRead = (((((0x1 | 0x2) | 0x40) | 0x80) | 0x200) | 0x800),
	Present = 0,
	Predication = 0x200,
	VideoDecodeReaad = 0x10000,
	VideoDecodeWrite = 0x20000,
	VideoProcessRead = 0x40000,
	VideoProcessWrite = 0x80000,
	VideoEncodeRead = 0x200000,
	VideoEncodeWrite = 0x800000
};


/* Table */

enum class EFRIRootDescriptorRangeType
{
	SRV,
	CBV,
	UAV,
	Sampler
};

struct FRIDescriptorRange
{
	EFRIRootDescriptorRangeType RangeType;
	uint32 NumDescriptors;
	uint32 BaseShaderRegister;
	uint32 RegisterSpace;
	uint32 OffsetInDescriptorsFromTableStart;
};

struct FRIRootDescriptorTable
{
	uint32 NumRanges;
	FRIDescriptorRange* Ranges;
};

/* Descriptor */

struct FRIRootDescriptor
{
	uint32 ShaderRegister;
	uint32 RegisterSpace;

};

/* Constant */

struct FRIRootConstants
{

};


/* Parameter */

struct FRIRootParameter
{
	EFRIRootParameterType ParamType;
	EFRIShaderVisibility ShaderVisibility;

	union
	{
		FRIRootDescriptorTable DescriptorTable;
		FRIRootDescriptor Descriptor;
		FRIRootConstants Constants;
	};

	FRIRootParameter(EFRIRootParameterType ParamType, EFRIShaderVisibility ShaderVisibility, FRIRootDescriptorTable DescriptorTable) :
		ParamType(ParamType),
		ShaderVisibility(ShaderVisibility),
		DescriptorTable(DescriptorTable)
	{}

	FRIRootParameter(EFRIRootParameterType ParamType, EFRIShaderVisibility ShaderVisibility, FRIRootDescriptor Descriptor) :
		ParamType(ParamType),
		ShaderVisibility(ShaderVisibility),
		Descriptor(Descriptor)
	{}
};

/* Sampler */

struct FRIStaticSampler
{
	EFRITextureFilter Filter;
	EFRITextureAddress AddressU;
	EFRITextureAddress AddressV;
	EFRITextureAddress AddressW;
	uint32 ShaderRegister;
	uint32 RegisterSpace;
	//Color32 BorderColor = Color32();

	FRIStaticSampler(EFRITextureFilter Filter, EFRITextureAddress AddressU, EFRITextureAddress AddressV, EFRITextureAddress AddressW, uint32 ShaderRegister, uint32 RegisterSpace) :
		Filter(Filter),
		AddressU(AddressU),
		AddressV(AddressV),
		AddressW(AddressW),
		ShaderRegister(ShaderRegister),
		RegisterSpace(RegisterSpace)
	{}

};

enum EShaderFrequency
{
	SF_Vertex = 0,
	SF_Pixel = 1,
	SF_Geometry = 2,
	SF_Hull = 3,
	SF_Domain = 4,
	SF_Compute = 5,
	SF_ShaderStages = 6
};
