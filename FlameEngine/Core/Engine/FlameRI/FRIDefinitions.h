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
	Float = 0x1406,
	Int = 0x1404
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

enum class EFRITextureWrapMode
{
	Repeat = 0x2901,
	ClampEdge,
	ClampBorder,
	MirroredRepeat,
	MirroredClampEdge
};

enum class EFRITextureParamName
{
	MagFilter = 0x2800,
	MinFilter = 0x2801,
	WrapS = 0x2802,
	WrapT = 0x2803,
	WrapR = 0x8702,
	AnisotropyLevel = 0x84FE
};

enum class EFRITextureFilterMode
{
	Nearest = 0x2600,
	Bilinear = 0x2601,
	Trilinear = 0x2703,
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

enum class EFRIAlphaBlend
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
