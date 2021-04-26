#pragma once




enum class EFRIUniformBufferParameterType
{
	Int32,
	UInt32,
	Float,
	Bool,

	FVector2,
	FVector3,
	FVector4,
	FMatrix2,
	FMatrix3,
	FMatrix4,

	IVector2,
	IVector3,
	IVector4,
	IMatrix2,
	IMatrix3,
	IMatrix4,
	MaxUniformTypes
};

enum class EFRIUniformSamplerType
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

enum class EFRIResourceShaderType
{
	Vertex_Shader,
	Fragment_Shader,
	Geometry_Shader,
	Hull_Shader,
	Domain_Shader,
	Compute_Shader
};

enum class EFRIVertexDeclerationAttributeType : unsigned int
{
	Float = 0x1406,
	Int = 0x1404
};

enum class EFRIPrimitiveType
{
	Lines = 0x0001,
	Triangles = 0x0004
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



enum class EFRITextureChannelStorage
{
	RG16F = 0x822F,
	RGB16F = 0x881B, 
	RGBA16F = 0x881A,

	RG32F = 0x8230,
	RGB32F = 0x8815,
	RGBA32F = 0x8814
};

enum class EFRITextureChannels
{
	R,
	G,
	B,
	RG,
	RGB = 0x1907,
	RGBA = 0x1908
};

enum class EFRITexturePixelStorage
{
	Float = 0x1406,
	UnsignedByte = 0x1401
};