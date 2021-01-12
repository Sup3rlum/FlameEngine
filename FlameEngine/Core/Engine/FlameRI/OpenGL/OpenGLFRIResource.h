#pragma once

#include "../FRIResource.h"
#include "OpenGL.h"


struct FGLResourceTexture2D : FResourceTexture2D
{
	GLuint textureType;
	GLuint textureHandle;
	GLuint internalColorFormat;
	GLuint dataFormat;

	FGLResourceTexture2D(
		uint32 width, 
		uint32 height, 
		uint32 SampleCount, 
		GLuint internalColorFormat, 
		uint32 channels = 4, 
		GLuint dataFormat = GL_FLOAT, 
		FResourceCreationDescriptor Data = FResourceCreationDescriptor(NULL,0), 
		FResourceTextureParameterBuffer paramBuffer = FResourceTextureParameterBuffer(FArray<FResourceTextureParameterBuffer::FTextureParameterBufferParameter>())
	) : 
		FResourceTexture2D(width, height, SampleCount),
		dataFormat(dataFormat)

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
			glTexImage2D(textureType, 0, internalColorFormat, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, dataFormat, Data.DataArray);
			glGenerateMipmap(textureType);

			for (int i = 0; i < paramBuffer.Data.Length(); i++)
			{
				if (paramBuffer.Data[i].paramType == EFRITextureParameterBufferParameterType::ENUM)
				{
					glTexParameteri(textureType, paramBuffer.Data[i].Param, paramBuffer.Data[i].EnumParam);
				}
				if (paramBuffer.Data[i].paramType == EFRITextureParameterBufferParameterType::FLOAT)
				{
					glTexParameterf(textureType, paramBuffer.Data[i].Param, paramBuffer.Data[i].FloatParam);
				}
			}
		}
	}

	
};


struct FGLResourceTexture3D : FResourceTexture3D
{
	GLuint textureHandle;
	GLuint internalColorFormat;

	// TODO: Add support for 3DTex
};


struct FGLResourceTextureCubeMap : FResourceTextureCubeMap
{
public:
	GLuint textureHandle;
	GLuint internalColorFormat;

	// TODO: Add support for CubeTex

};




struct FGLResourceVertexDeclaration : FResourceVertexDeclaration
{
	FGLResourceVertexDeclaration(FArray<FResourceVertexDeclarationComponent> DeclarationElements) : FResourceVertexDeclaration(DeclarationElements)
	{

		for (int i = 0; i < DeclarationElements.Length(); i++)
		{

			glVertexAttribPointer
			(
				DeclarationElements[i].AttribNumber,
				DeclarationElements[i].ByteSize,
				DeclarationElements[i].Type,
				DeclarationElements[i].Normalized,
				DeclarationElements[i].Stride,
				DeclarationElements[i].Offset
			);
			glEnableVertexAttribArray(DeclarationElements[i].AttribNumber);
		}
	}
};

struct FGLResourceVertexBuffer : FResourceVertexBuffer
{
	GLuint vertexBufferHandle;
	GLuint vertexArrayBindingHandle;

	FGLResourceVertexBuffer(uint32 Size, uint32 Usage, FResourceCreationDescriptor Data) : FResourceVertexBuffer(Size, Usage) 
	{


		// TODO: fix this baloney
		glGenVertexArrays(1, &vertexArrayBindingHandle);
		glBindVertexArray(vertexArrayBindingHandle);

		OpenGL::GenBuffers(1, &vertexBufferHandle);
		OpenGL::BufferData(vertexBufferHandle, GL_ARRAY_BUFFER, Data.DataArray, Data.ByteSize, GL_STATIC_DRAW);		
	}
};





struct FGLResourceIndexBuffer : FResourceIndexBuffer
{
	GLuint indexBufferHandle;

	FGLResourceIndexBuffer(uint32 IndexCount, uint32 Usage, FResourceCreationDescriptor Data) : FResourceIndexBuffer(IndexCount, Usage)
	{

		OpenGL::GenBuffers(1, &indexBufferHandle);
		OpenGL::BufferData(indexBufferHandle, GL_ELEMENT_ARRAY_BUFFER, Data.DataArray, Data.ByteSize, GL_STATIC_DRAW);
	}
};




struct FGLResourceUniformBuffer : FResourceUniformBuffer
{
	GLuint uniformBufferHandle;

	// TODO: Add support 

};


struct FGLResourceFrameBuffer : FResourceFrameBuffer
{
	GLuint frameBufferHandle;
	GLuint depthBufferHandle;

	enum EResourceFBTextureAttachmentType
	{
		COLOR_ATTACHMENT,
		DEPTH_ATTACHMENT
	};

	struct ResourceFrameBufferTextureAttachment
	{
		FGLResourceTexture2D* textureParamRef;
		EResourceFBTextureAttachmentType attachmentType;
	};


	FGLResourceFrameBuffer(FArray<ResourceFrameBufferTextureAttachment> textureAttachments, bool enableDepthRenderBuffer = true) : FResourceFrameBuffer(0, 0)
	{

		if (textureAttachments.Length() > 16)
		{
			// TODO : too many attachments
		}

		glGenFramebuffers(1, &frameBufferHandle);

		int colorAttachmentCurrent = GL_COLOR_ATTACHMENT0;
		bool hasDepthAttchment = false;

		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferHandle);

		for (int i = 0; i < textureAttachments.Length(); i++)
		{
			int attachment = 0;

			if (textureAttachments[i].attachmentType == COLOR_ATTACHMENT)
			{
				attachment = colorAttachmentCurrent++;
			}
			else if (textureAttachments[i].attachmentType == DEPTH_ATTACHMENT)
			{
				if (hasDepthAttchment)
				{
					// TODO: Error - too many depth attachments
				}

				attachment = GL_DEPTH_ATTACHMENT;
				hasDepthAttchment = true;
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textureAttachments[i].textureParamRef->textureType, textureAttachments[i].textureParamRef->textureHandle, 0);
		}

		if (enableDepthRenderBuffer)
		{
			glGenRenderbuffers(1, &depthBufferHandle);
			glBindRenderbuffer(GL_RENDERBUFFER, depthBufferHandle);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 100, 100);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferHandle);
		}

	}



};


struct FGLResourceRenderQuery : FResourceRenderQuery
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

	TGLResourceShaderType(FString Name, const FArray<byte>& binCode) : FResourceVertexShader(Name)
	{

		if (!OpenGL::DrvSupportsBinaryShaderFormat())
		{
			// Driver does not support compiling shader pipelines from binary sources!
		}

		shaderHandle = glCreateShader(ShaderType);

		glShaderBinary(1, &shaderHandle, 0, binCode.Begin(), binCode.ByteSize());
		glCompileShader(shaderHandle);

		int32 vSuccess;
		FStaticANSIString<512> vInfo;

		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &vSuccess);
		if (!vSuccess)
		{
			glGetShaderInfoLog(shaderHandle, 512, NULL, vInfo);
			//FLAME_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + STRING(vInfo));
		};

	}

	uint32 GetResource() const
	{
		return shaderHandle;
	}
};


typedef TGLResourceShaderType<GL_VERTEX_SHADER, FResourceVertexShader> FGLResourceVertexShader;
typedef TGLResourceShaderType<GL_FRAGMENT_SHADER, FResourcePixelShader> FGLResourcePixelShader;
typedef TGLResourceShaderType<GL_GEOMETRY_SHADER, FResourceGeometryShader> FGLResourceGeometryShader;
typedef TGLResourceShaderType<GL_TESS_CONTROL_SHADER, FResourceHullShader> FGLResourceHullShader;
typedef TGLResourceShaderType<GL_TESS_EVALUATION_SHADER, FResourceDomainShader> FGLResourceDomainShader;
typedef TGLResourceShaderType<GL_COMPUTE_SHADER, FResourceComputeShader> FGLResourceComputeShader;



struct FGLResourceShaderPipeline : public FResourceShaderPipeline
{
	GLuint pipelineHandle;


	FGLResourceShaderPipeline(FResourceShaderPipelineCreationDescriptor descriptor) : FResourceShaderPipeline(descriptor)
	{
		pipelineHandle = glCreateProgram();

		if (descriptor.NumShaders >= 6)
		{
			// ERROR: Too many shaders provided - Invalid descriptor

		}

		for (int i = 0; i < descriptor.NumShaders; i++)
		{
			glAttachShader(pipelineHandle, descriptor.ShaderArray[i].GetResource());
		}
		glLinkProgram(pipelineHandle);

		int lSuccess;
		FStaticANSIString<512> lInfo;

		glGetProgramiv(pipelineHandle, GL_LINK_STATUS, &lSuccess);
		if (!lSuccess)
		{
			glGetProgramInfoLog(pipelineHandle, 512, NULL, lInfo);
			//FLAME_ERROR("ERROR::SHADER::LINKING_FAILED\n" + STRING(lInfo));
		}
	}
};