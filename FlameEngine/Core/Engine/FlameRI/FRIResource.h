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
struct FResourceTexture2DArray : public FResourceObject
{
	uint32 Width;
	uint32 Height;
	uint32 NumLayers;

	FResourceTexture2DArray(uint32 Width, uint32 Height, uint32 NumLayers) :
		Width(Width),
		Height(Height),
		NumLayers(NumLayers)
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


struct FUniformParameter
{
	uint32 Location;
	uint32 ArrayCount;

	union
	{
		uint32 UintParam;
		int32 IntParam;
		float FloatParam;
		bool BoolParam;

		const float* FloatVParam;
		const int* IntVParam;

	};


	FUniformParameter(uint32 Location, uint32 val) : Location(Location), paramType(EFRIUniformBufferParameterType::UInt32), UintParam(val), ArrayCount(1) {}
	FUniformParameter(uint32 Location, int32 val) : Location(Location), paramType(EFRIUniformBufferParameterType::Int32), IntParam(val), ArrayCount(1) {}
	FUniformParameter(uint32 Location, float val) : Location(Location), paramType(EFRIUniformBufferParameterType::Float), FloatParam(val), ArrayCount(1) {}

	FUniformParameter(uint32 Location, FVector4 vec) : Location(Location), paramType(EFRIUniformBufferParameterType::FVector4), FloatVParam(&vec[0]), ArrayCount(1) {}
	FUniformParameter(uint32 Location, FVector3 vec) : Location(Location), paramType(EFRIUniformBufferParameterType::FVector3), FloatVParam(&vec[0]), ArrayCount(1) {}
	FUniformParameter(uint32 Location, FVector2 vec) : Location(Location), paramType(EFRIUniformBufferParameterType::FVector2), FloatVParam(&vec[0]), ArrayCount(1) {}

	FUniformParameter(uint32 Location, FMatrix4 mat) : Location(Location), paramType(EFRIUniformBufferParameterType::FMatrix4), FloatVParam(&mat[0][0]), ArrayCount(1) {}
	FUniformParameter(uint32 Location, FMatrix3 mat) : Location(Location), paramType(EFRIUniformBufferParameterType::FMatrix3), FloatVParam(&mat[0][0]), ArrayCount(1) {}
	FUniformParameter(uint32 Location, FMatrix2 mat) : Location(Location), paramType(EFRIUniformBufferParameterType::FMatrix2), FloatVParam(&mat[0][0]), ArrayCount(1) {}



	FUniformParameter(uint32 Location, const FArray<FVector4>& vec) : Location(Location), paramType(EFRIUniformBufferParameterType::FVector4), FloatVParam(&vec[0][0]), ArrayCount(vec.Length()) {}
	FUniformParameter(uint32 Location, const FArray<FVector3>& vec) : Location(Location), paramType(EFRIUniformBufferParameterType::FVector3), FloatVParam(&vec[0][0]), ArrayCount(vec.Length()) {}
	FUniformParameter(uint32 Location, const FArray<FVector2>& vec) : Location(Location), paramType(EFRIUniformBufferParameterType::FVector2), FloatVParam(&vec[0][0]), ArrayCount(vec.Length()) {}
									   
	FUniformParameter(uint32 Location, const FArray<FMatrix4>& mat) : Location(Location), paramType(EFRIUniformBufferParameterType::FMatrix4), FloatVParam(&mat[0][0][0]), ArrayCount(mat.Length()) {}
	FUniformParameter(uint32 Location, const FArray<FMatrix3>& mat) : Location(Location), paramType(EFRIUniformBufferParameterType::FMatrix3), FloatVParam(&mat[0][0][0]), ArrayCount(mat.Length()) {}
	FUniformParameter(uint32 Location, const FArray<FMatrix2>& mat) : Location(Location), paramType(EFRIUniformBufferParameterType::FMatrix2), FloatVParam(&mat[0][0][0]), ArrayCount(mat.Length()) {}



	FUniformParameter(const FUniformParameter& other) :
		Location(other.Location),
		paramType(other.paramType),
		FloatVParam(other.FloatVParam)
	{

	}

	EFRIUniformBufferParameterType paramType;
private:
	FUniformParameter();
};


struct FUniformSampler
{
	uint32 Unit;
	EFRIUniformSamplerType samplerType;

	union
	{
		FResourceTexture2D* Param2D;
		FResourceTexture3D* Param3D;
		FResourceTextureCubeMap* ParamCube;
		FResourceTexture2DArray* Param2DArray;
	};


	FUniformSampler(uint32 unit, FResourceTexture2D* val) : Unit(unit), samplerType(EFRIUniformSamplerType::TSampler2D), Param2D(val) {}
	FUniformSampler(uint32 unit, FResourceTexture3D* val) : Unit(unit), samplerType(EFRIUniformSamplerType::TSampler3D), Param3D(val) {}
	FUniformSampler(uint32 unit, FResourceTextureCubeMap* val) : Unit(unit), samplerType(EFRIUniformSamplerType::TSamplerCube), ParamCube(val) {}
	FUniformSampler(uint32 unit, FResourceTexture2DArray* val) : Unit(unit), samplerType(EFRIUniformSamplerType::TSampler2DArray), Param2DArray(val) {}

private:
	FUniformSampler();
};



struct FResourceUniformBuffer : public FResourceObject
{

	FArray<FUniformParameter> Data;


	FResourceUniformBuffer(FArray<FUniformParameter> Data) :
		Data(Data)
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


struct FResourceTextureParameterBuffer : public FResourceObject
{


	FArray<FTextureParameterBufferParameter> Data;

	FResourceTextureParameterBuffer(const FArray<FTextureParameterBufferParameter>& Data) :
		Data(Data)
	{

	}
	FResourceTextureParameterBuffer(const FResourceTextureParameterBuffer& other) :
		Data(other.Data)
	{

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
	FResourceShaderBase** ShaderArray;

	FResourceShaderPipelineCreationDescriptor(uint32 NumShaders, FResourceShaderBase** ShaderArray) :
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
	uint32 Length;
	EFRIVertexDeclerationAttributeType Type;
	EFRIBool Normalized;
	uint32 Stride;
	uint32 Offset;

	FResourceVertexDeclarationComponent(uint32 attribNumber, uint32 length, EFRIVertexDeclerationAttributeType type, EFRIBool norm, uint32 stride, uint32 offset) :
		AttribNumber(attribNumber),
		Type(type),
		Normalized(norm),
		Stride(stride),
		Offset(offset),
		Length(length)
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

struct FResourceFrameBufferTextureAttachment
{
	union
	{
		FResourceTexture2D* Param2D;
		FResourceTexture3D* Param3D;
		FResourceTextureCubeMap* ParamCube;
		FResourceTexture2DArray* Param2DArray;
	};

	EResourceFBTextureAttachmentType attachmentType;
	EFRIUniformSamplerType textureType;


	FResourceFrameBufferTextureAttachment(FResourceTexture2D* texture, EResourceFBTextureAttachmentType attachmentType) : Param2D(texture), attachmentType(attachmentType), textureType(EFRIUniformSamplerType::TSampler2D) {}
	FResourceFrameBufferTextureAttachment(FResourceTexture2DArray* texture, EResourceFBTextureAttachmentType attachmentType) : Param2DArray(texture), attachmentType(attachmentType), textureType(EFRIUniformSamplerType::TSampler2DArray) {}
};


struct FResourceTextureColorDescriptor
{
	EFRITextureChannelStorage storageFormat;
	EFRITextureChannels channelFormat;
	EFRITexturePixelStorage pixelStorage;

	FResourceTextureColorDescriptor(EFRITextureChannelStorage storageFormat, EFRITextureChannels channelFormat, EFRITexturePixelStorage pixelStorage) :
		storageFormat(storageFormat),
		channelFormat(channelFormat),
		pixelStorage(pixelStorage)
	{

	}


};



struct FRIByte : FResourceArrayInterface
{
	byte _Internal;
};