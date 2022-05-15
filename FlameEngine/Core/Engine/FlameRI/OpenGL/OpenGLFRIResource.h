#pragma once

#include "../FRIResource.h"
#include "OpenGL.h"



struct FGLTextureBase
{
	GLuint textureType;
	GLuint textureHandle;

	FGLTextureBase(GLuint type, GLuint handle) :
		textureType(type),
		textureHandle(handle)
	{
	}
};

struct FGLResourceTexture2D : FGLTextureBase, FRITexture2D
{

	GLuint internalColorFormat;
	GLuint dataFormat;

	FGLResourceTexture2D(
		uint32 width, 
		uint32 height, 
		uint32 SampleCount, 
		GLuint internalColorFormat, 
		GLuint channels = GL_RGBA, 
		GLuint dataFormat = GL_FLOAT, 
		FRICreationDescriptor Data = FRICreationDescriptor(NULL,0), 
		FRITextureParameterBuffer paramBuffer = FRITextureParameterBuffer(FArray<FTextureParameterBufferParameter>())
	) : 
		FRITexture2D(width, height, SampleCount),
		FGLTextureBase(0, 0),
		dataFormat(dataFormat),
		internalColorFormat(internalColorFormat)

	{

		OpenGL::GenTextures(1, &textureHandle);


		if (SampleCount > 0)
		{
			textureType = GL_TEXTURE_2D_MULTISAMPLE;

			// TODO: add support for multisample textures (mainly buffers for multisampled FB/RTs)
		}
		else
		{
			textureType = GL_TEXTURE_2D;


			glBindTexture(textureType, textureHandle);
			//glTexImage2D(textureType, 0, internalColorFormat, width, height, 0, channels, dataFormat, Data.DataArray);

			glTexStorage2D(textureType, 1, internalColorFormat, width, height);
			if (Data.DataArray)
			{
				glTexSubImage2D(textureType, 0, 0, 0, width, height, channels, dataFormat, Data.DataArray);
				glGenerateMipmap(textureType);
			}

			for (int i = 0; i < paramBuffer.Data.Length(); i++)
			{
				if (paramBuffer.Data[i].paramType == EFRITextureParameterBufferParameterType::Enum)
				{
					glTexParameteri(textureType, paramBuffer.Data[i].ParamName, paramBuffer.Data[i].EnumParam);
				}
				if (paramBuffer.Data[i].paramType == EFRITextureParameterBufferParameterType::Float)
				{
					glTexParameterf(textureType, paramBuffer.Data[i].ParamName, paramBuffer.Data[i].FloatParam);
				}
			}
		}
	}

	~FGLResourceTexture2D()
	{
		glDeleteTextures(1, &textureHandle);
	}
};


struct FGLResourceTexture3D : FRITexture3D
{
	GLuint textureHandle;
	GLuint internalColorFormat;

	// TODO: Add support for 3DTex
};


struct FGLResourceTextureCubeMap : FRITextureCubeMap
{
public:
	GLuint textureHandle;
	GLuint internalColorFormat;

	// TODO: Add support for CubeTex

};

struct FGLResourceTexture2DArray : FGLTextureBase, FRITexture2DArray
{
	GLuint internalColorFormat;
	GLuint dataFormat;


	FGLResourceTexture2DArray(
		uint32 width,
		uint32 height,
		uint32 NumLayers,
		GLuint internalColorFormat,
		GLuint channels = GL_RGBA,
		GLuint dataFormat = GL_FLOAT,
		FRICreationDescriptor Data = FRICreationDescriptor(NULL, 0),
		FRITextureParameterBuffer paramBuffer = FRITextureParameterBuffer(FArray<FTextureParameterBufferParameter>())
	) :
		FRITexture2DArray(width, height, NumLayers),
		FGLTextureBase(0, 0),
		dataFormat(dataFormat),
		internalColorFormat(internalColorFormat)
	{
		OpenGL::GenTextures(1, &textureHandle);


		textureType = GL_TEXTURE_2D_ARRAY;


		glBindTexture(textureType, textureHandle);
		glTexImage3D(textureType, 0, internalColorFormat, width, height, NumLayers, 0, channels, dataFormat, Data.DataArray);
		glGenerateMipmap(textureType);

		for (int i = 0; i < paramBuffer.Data.Length(); i++)
		{
			if (paramBuffer.Data[i].paramType == EFRITextureParameterBufferParameterType::Enum)
			{
				glTexParameteri(textureType, paramBuffer.Data[i].ParamName, paramBuffer.Data[i].EnumParam);
			}
			if (paramBuffer.Data[i].paramType == EFRITextureParameterBufferParameterType::Float)
			{
				glTexParameterf(textureType, paramBuffer.Data[i].ParamName, paramBuffer.Data[i].FloatParam);
			}
		}

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 1);

	}
	~FGLResourceTexture2DArray()
	{
		glDeleteTextures(1, &textureHandle);
	}
};




struct FGLResourceVertexDeclaration : FRIVertexDeclaration
{
	FGLResourceVertexDeclaration(FArray<FRIInputDesc> DeclarationElements) : FRIVertexDeclaration(DeclarationElements)
	{

	}
};

struct FGLResourceVertexBuffer : FRIVertexBuffer
{
	GLuint vertexBufferHandle;
	GLuint vertexArrayBindingHandle;

	FGLResourceVertexBuffer(uint32 Size, uint32 Usage, FRICreationDescriptor Data) : FRIVertexBuffer(Size, Usage, EFRIAccess::None) 
	{


		// TODO: fix this baloney
		glGenVertexArrays(1, &vertexArrayBindingHandle);
		glBindVertexArray(vertexArrayBindingHandle);

		OpenGL::GenBuffers(1, &vertexBufferHandle);
		OpenGL::BufferData(vertexBufferHandle, GL_ARRAY_BUFFER, Data.DataArray, Data.ByteSize, GL_STATIC_DRAW);		
	}
	~FGLResourceVertexBuffer()
	{
		glDeleteBuffers(1, &vertexBufferHandle);
		glDeleteVertexArrays(1, &vertexArrayBindingHandle);
	}
};





struct FGLResourceIndexBuffer : FRIIndexBuffer
{
	GLuint indexBufferHandle;

	FGLResourceIndexBuffer(uint32 IndexCount, uint32 Usage, FRICreationDescriptor Data) : FRIIndexBuffer(IndexCount, Usage, EFRIAccess::None)
	{

		OpenGL::GenBuffers(1, &indexBufferHandle);
		OpenGL::BufferData(indexBufferHandle, GL_ELEMENT_ARRAY_BUFFER, Data.DataArray, Data.ByteSize, GL_DYNAMIC_DRAW);
	}

	~FGLResourceIndexBuffer()
	{
		glDeleteBuffers(1, &indexBufferHandle);
	}

};




struct FGLResourceUniformBuffer : FRIUniformBuffer
{
	GLuint bufferHandle;

	FGLResourceUniformBuffer(FRICreationDescriptor Data) :
		FRIUniformBuffer(Data.ByteSize, 0, 0, EFRIAccess::None)
	{
		OpenGL::GenBuffers(1, &bufferHandle);

		glBindBuffer(GL_UNIFORM_BUFFER, bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, Data.ByteSize, Data.DataArray, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, NULL);
	}

	~FGLResourceUniformBuffer()
	{
		glDeleteBuffers(1, &bufferHandle);
	}
};


struct FGLResourceFrameBuffer : FRIFrameBuffer
{
	GLuint frameBufferHandle;
	GLuint depthBufferHandle;



	FGLResourceFrameBuffer(FArray<FRIFrameBufferAttachment> textureAttachments, bool enableDepthRenderBuffer = true) :
		FRIFrameBuffer(0, 0),
		frameBufferHandle(0),
		depthBufferHandle(0)
	{

		/*if (textureAttachments.Length() > 16 || textureAttachments.Length() < 1)
		{
			// TODO : too many attachments

		}

		glGenFramebuffers(1, &frameBufferHandle);

		int colorAttachmentCurrent = GL_COLOR_ATTACHMENT0;
		bool hasDepthAttchment = false;

		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferHandle);


		FArray<GLenum> attachmentArray;

		for (int i = 0; i < textureAttachments.Length(); i++)
		{
			int attachment = 0;

			if (textureAttachments[i].attachmentType == EResourceFBTextureAttachmentType::Color)
			{
				attachment = ++colorAttachmentCurrent;
			}
			else if (textureAttachments[i].attachmentType == EResourceFBTextureAttachmentType::Depth)
			{
				if (hasDepthAttchment)
				{
					// TODO: Error - too many depth attachments
				}

				attachment = GL_DEPTH_ATTACHMENT;
				hasDepthAttchment = true;
			}


			// Array textures arent abound using glDrawBuffers, instead using cmdList.SetFramebufferTextureLayer -> glFramebufferTextureLayer
			if (textureAttachments[i].textureType != EFRIUniformSamplerType::TSampler2DArray)
			{
				attachmentArray.Add(attachment);
			}

			GLuint texHandle;

			switch (textureAttachments[i].textureType)
			{
				case EFRIUniformSamplerType::TSampler2D:
				{
					texHandle = ((FGLResourceTexture2D*)textureAttachments[i].Param2D)->textureHandle;
					break;
				}
				case EFRIUniformSamplerType::TSampler2DArray:
				{
					texHandle = ((FGLResourceTexture2DArray*)textureAttachments[i].Param2DArray)->textureHandle;
					break;
				}
			}


			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texHandle, 0);
		}

		if (enableDepthRenderBuffer)
		{
			GLuint width, height;

			switch (textureAttachments[0].textureType)
			{
				case EFRIUniformSamplerType::TSampler2D:
				{
					width = ((FGLResourceTexture2D*)textureAttachments[0].Param2D)->Width;
					height = ((FGLResourceTexture2D*)textureAttachments[0].Param2D)->Height;
					break;
				}
				case EFRIUniformSamplerType::TSampler2DArray:
				{
					width = ((FGLResourceTexture2DArray*)textureAttachments[0].Param2DArray)->Width;
					height = ((FGLResourceTexture2DArray*)textureAttachments[0].Param2DArray)->Height;
					break;
				}
			}


			glGenRenderbuffers(1, &depthBufferHandle);
			glBindRenderbuffer(GL_RENDERBUFFER, depthBufferHandle);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferHandle);
		}

		if (attachmentArray.Length() > 0)
		{
			glDrawBuffers(attachmentArray.Length(), attachmentArray.Begin());
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
	}


	FRITexture2D* GetDepthBuffer()
	{
		return 0;
	}

	~FGLResourceFrameBuffer()
	{
		glDeleteFramebuffers(1, &frameBufferHandle);
	}
};


struct FGLResourceRenderQuery : FRIRenderQuery
{
	FGLResourceRenderQuery()
	{

	}
};


/*
*		SHADERS
*/


struct FGLResourceShaderBase
{
	GLuint shaderHandle;
};


template<GLenum ShaderType, typename TFRIBaseShaderType>
struct TGLResourceShaderType : public TFRIBaseShaderType, public FGLResourceShaderBase
{

	TGLResourceShaderType(const FArray<byte>& binCode)
	{
		// The incoming bytedata is a multi-byte pure GLSL string to be compiled

		if (!OpenGL::DrvSupportsBinaryShaderFormat())
		{
			// Driver does not support compiling shader pipelines from binary sources!
		}

		shaderHandle = glCreateShader(ShaderType);

		glShaderBinary(1, &shaderHandle, GL_SHADER_BINARY_FORMAT_SPIR_V, binCode.Begin(), binCode.ByteSize());
		glSpecializeShader(shaderHandle, "main", 0, 0, 0);

		int32 vSuccess;
		FStaticAnsiString<512> vInfo;

		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &vSuccess);
		if (!vSuccess)
		{
			glGetShaderInfoLog(shaderHandle, 512, NULL, vInfo);

			printf("ERROR::SHADER::COMPILATION\n\n");
			printf(vInfo.ToPlatformString());

		};

	}



	~TGLResourceShaderType()
	{
		glDeleteShader(shaderHandle);
	}

	uint32 GetResource() const
	{
		return shaderHandle;
	}
};


typedef TGLResourceShaderType<GL_VERTEX_SHADER, FRIVertexShader> FGLResourceVertexShader;
typedef TGLResourceShaderType<GL_FRAGMENT_SHADER, FRIPixelShader> FGLResourcePixelShader;
typedef TGLResourceShaderType<GL_GEOMETRY_SHADER, FRIGeometryShader> FGLResourceGeometryShader;
typedef TGLResourceShaderType<GL_TESS_CONTROL_SHADER, FRIHullShader> FGLResourceHullShader;
typedef TGLResourceShaderType<GL_TESS_EVALUATION_SHADER, FRIDomainShader> FGLResourceDomainShader;
typedef TGLResourceShaderType<GL_COMPUTE_SHADER, FRIComputeShader> FGLResourceComputeShader;



struct FGLResourceShaderPipeline : public FRIShaderPipeline
{
	GLuint pipelineHandle;


	FGLResourceShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor) : FRIShaderPipeline(descriptor)
	{
		pipelineHandle = glCreateProgram();

		if (descriptor.NumShaders >= 6)
		{
			// ERROR: Too many shaders provided - Invalid descriptor

		}

		for (int i = 0; i < descriptor.NumShaders; i++)
		{
			glAttachShader(pipelineHandle, descriptor.ShaderArray[i]->GetResource());
		}
		glLinkProgram(pipelineHandle);

		int lSuccess;
		FStaticAnsiString<512> lInfo;

		glGetProgramiv(pipelineHandle, GL_LINK_STATUS, &lSuccess);
		if (!lSuccess)
		{
			glGetProgramInfoLog(pipelineHandle, 512, NULL, lInfo);
			//FLAME_ERROR("ERROR::SHADER::LINKING_FAILED\n" + STRING(lInfo));
			//wprintf(FStringFormatter::Format(L"ERROR::SHADER::LINKING_FAILED\n\n%0", FString(lInfo.ToPlatformString())).ToPlatformString());
			printf("ERROR::SHADER::LINKING_FAILED\n\n");
			printf(lInfo.ToPlatformString());
		}
	}
	~FGLResourceShaderPipeline()
	{
		glDeleteProgram(pipelineHandle);
	}
};




