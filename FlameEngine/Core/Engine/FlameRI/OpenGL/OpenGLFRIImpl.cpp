#include "OpenGLFRI.h"

#include "FVXLToGLSL.h"

#include <iostream>


typedef void (*_UniformFuncPtr)(FUniformParameter* param);
typedef void (*_SamplerFuncPtr)(FUniformSampler* param);

FStaticArray<_UniformFuncPtr, (size_t)EFRIUniformBufferParameterType::MaxUniformTypes> _FUniformFuncPtrs;
FStaticArray<_SamplerFuncPtr, (size_t)EFRIUniformSamplerType::MaxSamplerTypes> _FSamplerFuncPtrs;

#define SET_ENUM_UNIFORM_FUNC_PTR(e, f) _FUniformFuncPtrs[(size_t)EFRIUniformBufferParameterType::e]	= [](FUniformParameter* fp) { f }
#define SET_ENUM_SAMPLER_FUNC_PTR(e, f) _FSamplerFuncPtrs[(size_t)EFRIUniformSamplerType::e]			= [](FUniformSampler* fp) { f }


FResourceVertexBuffer* OpenGLFRIDynamicAllocator::DynamicCreateVertexBuffer(uint32 Size, uint32 Usage, FResourceCreationDescriptor resourceDescriptor)
{
	FGLResourceVertexBuffer* fglVertexBuffer = new FGLResourceVertexBuffer(Size, Usage, resourceDescriptor);


	return fglVertexBuffer;
}

FResourceIndexBuffer* OpenGLFRIDynamicAllocator::DynamicCreateIndexBuffer(uint32 IndexCount, uint32 Usage, FResourceCreationDescriptor resourceDescriptor)
{
	FGLResourceIndexBuffer* fglIndexBuffer = new FGLResourceIndexBuffer(IndexCount, Usage, resourceDescriptor);


	return fglIndexBuffer;
}


FResourceTexture2D* OpenGLFRIDynamicAllocator::DynamicCreateTexture2D(uint32 width, uint32 height, uint32 sampleCount, FResourceTextureColorDescriptor colorDescriptor, FResourceCreationDescriptor resourceDescriptor)
{
	FGLResourceTexture2D* fglTex2D = new FGLResourceTexture2D(width, height, sampleCount, (GLuint)colorDescriptor.storageFormat, (GLuint)colorDescriptor.channelFormat, (GLuint)colorDescriptor.pixelStorage, resourceDescriptor);

	return fglTex2D;
}

FResourceTexture3D* OpenGLFRIDynamicAllocator::DynamicCreateTexture3D(uint32 width, uint32 height, uint32 depth, FResourceCreationDescriptor resourceDescriptor)
{
	//FGLResourceTexture3D* fglTex2D = new FGLResourceTexture3D(width, height, sampleCount, GL_RGBA32F, 4, GL_FLOAT, resourceDescriptor);
	
	// TODO: implement

	return NULL;
}


FResourceTexture2DArray* OpenGLFRIDynamicAllocator::DynamicCreateTexture2DArray(uint32 width, uint32 height, uint32 numLayers, FResourceTextureColorDescriptor colorDescriptor, FResourceCreationDescriptor resourceDescriptor)
{
	return new FGLResourceTexture2DArray(width, height, numLayers, (GLuint)colorDescriptor.storageFormat, (GLuint)colorDescriptor.channelFormat, (GLuint)colorDescriptor.pixelStorage, resourceDescriptor);
}




FResourceShaderPipeline* OpenGLFRIDynamicAllocator::DynamicCreateShaderPipeline(FResourceShaderPipelineCreationDescriptor descriptor)
{
	FGLResourceShaderPipeline* fglShaderPipeline = new FGLResourceShaderPipeline(descriptor);

	return fglShaderPipeline;
}


FResourceVertexShader* OpenGLFRIDynamicAllocator::DynamicCreateVertexShader(const FAnsiString& binCode)
{
	return new FGLResourceVertexShader("VertexShader", binCode);
}
FResourcePixelShader* OpenGLFRIDynamicAllocator::DynamicCreatePixelShader(const FAnsiString& binCode)
{
	return new FGLResourcePixelShader("PixelShader", binCode);
}

FResourceGeometryShader* OpenGLFRIDynamicAllocator::DynamicCreateGeometryShader(const FAnsiString& binCode)
{
	return new FGLResourceGeometryShader("GeometryShader", binCode);
}

FResourceHullShader* OpenGLFRIDynamicAllocator::DynamicCreateHullShader(const FAnsiString& binCode)
{
	return new FGLResourceHullShader("HullShader", binCode);
}
FResourceDomainShader* OpenGLFRIDynamicAllocator::DynamicCreateDomainShader(const FAnsiString& binCode)
{
	return new FGLResourceDomainShader("HullShader", binCode);
}
FResourceComputeShader* OpenGLFRIDynamicAllocator::DynamicCreateComputeShader(const FAnsiString& binCode)
{
	return new FGLResourceComputeShader("ComputeShader", binCode);
}




FResourceFrameBuffer* OpenGLFRIDynamicAllocator::DynamicCreateFrameBuffer(FArray<FResourceFrameBufferTextureAttachment> textureAttachments, bool enableDepthRenderBuffer)
{
	return new FGLResourceFrameBuffer(textureAttachments, enableDepthRenderBuffer);
}


void OpenGLFRIDynamicAllocator::AttachVertexDeclaration(FResourceVertexDeclaration declaration)
{
	for (int i = 0; i < declaration.DeclarationElements.Length(); i++)
	{
		if (declaration.DeclarationElements[i].Type == EFRIVertexDeclerationAttributeType::Float)
		{
			glVertexAttribPointer
			(
				declaration.DeclarationElements[i].AttribNumber,
				declaration.DeclarationElements[i].Length,
				(int32)declaration.DeclarationElements[i].Type,
				(GLboolean)declaration.DeclarationElements[i].Normalized,
				declaration.DeclarationElements[i].Stride,
				(void*)declaration.DeclarationElements[i].Offset
			);
		}
		else
		{
			glVertexAttribIPointer
			(
				declaration.DeclarationElements[i].AttribNumber,
				declaration.DeclarationElements[i].Length,
				(int32)declaration.DeclarationElements[i].Type,
				declaration.DeclarationElements[i].Stride,
				(void*)declaration.DeclarationElements[i].Offset
			);
		}
		glEnableVertexAttribArray(declaration.DeclarationElements[i].AttribNumber);
	}
}


FArray<byte> OpenGLFRIDynamicAllocator::PrecacheShader(FArray<byte> inData, EFRIResourceShaderType type)
{
	// TODO: Here we convert FVX shaders to GLSL so we can pass the straight to the GL driver and get the dynamic resource
	// inData is a multi-byte string

	
	return FArray<byte>();
}

void OpenGLFRIDynamicAllocator::BindFrameBuffer(FResourceFrameBuffer* frameBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, ((FGLResourceFrameBuffer*)frameBuffer)->frameBufferHandle);
}

void OpenGLFRIDynamicAllocator::UnbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void OpenGLFRIDynamicAllocator::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
{
	glViewport(x, y, width, height);
}
void OpenGLFRIDynamicAllocator::DrawPrimitives(uint32 elementType, uint32 elementCount)
{
	glDrawArrays(elementType, 0, elementCount);
}

void OpenGLFRIDynamicAllocator::DrawPrimitivesIndexed(uint32 elementType, uint32 elementCount, uint32 indexType, FResourceIndexBuffer* indexBuffer)
{	
	glDrawElements(elementType, indexBuffer->IndexCount, indexType, (void*)0);
}

void OpenGLFRIDynamicAllocator::SetShaderPipeline(FResourceShaderPipeline* shader)
{
	FGLResourceShaderPipeline* glshader = (FGLResourceShaderPipeline*)shader;

	glUseProgram(glshader->pipelineHandle);
}
void OpenGLFRIDynamicAllocator::SetShaderUniformBuffer(FResourceUniformBuffer* uniformBuffer)
{

}
void OpenGLFRIDynamicAllocator::SetShaderUniformParameter(FUniformParameter* parameter)
{
	(_FUniformFuncPtrs[(size_t)parameter->paramType])(parameter);
}

void OpenGLFRIDynamicAllocator::SetShaderSampler(FUniformSampler* sampler)
{
	(_FSamplerFuncPtrs[(size_t)sampler->samplerType])(sampler);
}

uint32 OpenGLFRIDynamicAllocator::GetShaderUniformParameterLocation(FResourceShaderPipeline* shader, FAnsiString uniformName)
{
	return glGetUniformLocation(((FGLResourceShaderPipeline*)shader)->pipelineHandle, uniformName.ToPlatformString());
}



void OpenGLFRIDynamicAllocator::SetGeometrySource(FResourceVertexBuffer* vertexBuffer)
{
	glBindVertexArray(((FGLResourceVertexBuffer*)vertexBuffer)->vertexArrayBindingHandle);
}


void OpenGLFRIDynamicAllocator::BeginScene()
{

}
void OpenGLFRIDynamicAllocator::EndScene()
{

}



void OpenGLFRIDynamicAllocator::BeginFrame()
{
	



}
void OpenGLFRIDynamicAllocator::EndFrame()
{
	
}


void OpenGLFRIDynamicAllocator::SetTextureParameterBuffer(FResourceTexture2D* texture, FResourceTextureParameterBuffer paramBuffer)
{
	FGLResourceTexture2D* tex = static_cast<FGLResourceTexture2D*>(texture);
	glBindTexture(tex->textureType, tex->textureHandle);

	for (int i = 0; i < paramBuffer.Data.Length(); i++)
	{
		if (paramBuffer.Data[i].paramType == EFRITextureParameterBufferParameterType::Enum)
		{
			glTexParameteri(tex->textureType, paramBuffer.Data[i].ParamName, paramBuffer.Data[i].EnumParam);
		}
		if (paramBuffer.Data[i].paramType == EFRITextureParameterBufferParameterType::Float)
		{
			glTexParameterf(tex->textureType, paramBuffer.Data[i].ParamName, paramBuffer.Data[i].FloatParam);
		}
	}


}


void OpenGLFRIDynamicAllocator::SetTextureParameterBuffer(FResourceTexture2DArray* texture, FResourceTextureParameterBuffer paramBuffer)
{
	FGLResourceTexture2DArray* tex = static_cast<FGLResourceTexture2DArray*>(texture);
	glBindTexture(tex->textureType, tex->textureHandle);

	for (int i = 0; i < paramBuffer.Data.Length(); i++)
	{
		if (paramBuffer.Data[i].paramType == EFRITextureParameterBufferParameterType::Enum)
		{
			glTexParameteri(tex->textureType, paramBuffer.Data[i].ParamName, paramBuffer.Data[i].EnumParam);
		}
		if (paramBuffer.Data[i].paramType == EFRITextureParameterBufferParameterType::Float)
		{
			glTexParameterf(tex->textureType, paramBuffer.Data[i].ParamName, paramBuffer.Data[i].FloatParam);
		}
	}


}

void OpenGLFRIDynamicAllocator::SetFramebufferTextureLayer(FResourceTexture2DArray* tex, uint32 layer)
{
	glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, static_cast<FGLResourceTexture2DArray*>(tex)->textureHandle, 0, layer);
}



bool OpenGLFRIContext::InitializeOpenGLUniformExtensions()
{
	SET_ENUM_UNIFORM_FUNC_PTR(Float,	glUniform1f(fp->Location, fp->FloatParam););
	SET_ENUM_UNIFORM_FUNC_PTR(Int32,	glUniform1i(fp->Location, fp->IntParam););
	SET_ENUM_UNIFORM_FUNC_PTR(UInt32,	glUniform1ui(fp->Location, fp->UintParam););

	SET_ENUM_UNIFORM_FUNC_PTR(FVector2,		glUniform2fv(fp->Location, fp->ArrayCount, fp->FloatVParam); );
	SET_ENUM_UNIFORM_FUNC_PTR(FVector3,		glUniform3fv(fp->Location, fp->ArrayCount, fp->FloatVParam); );
	SET_ENUM_UNIFORM_FUNC_PTR(FVector4,		glUniform4fv(fp->Location, fp->ArrayCount, fp->FloatVParam); );
	SET_ENUM_UNIFORM_FUNC_PTR(FMatrix4,		glUniformMatrix4fv(fp->Location, fp->ArrayCount, GL_FALSE, fp->FloatVParam););

	
	SET_ENUM_SAMPLER_FUNC_PTR(TSampler2D,

		glActiveTexture(GL_TEXTURE0 + fp->Unit);
		glBindTexture(GL_TEXTURE_2D, ((FGLResourceTexture2D*)fp->Param2D)->textureHandle);
		);


	SET_ENUM_SAMPLER_FUNC_PTR(TSampler2DArray,

		glActiveTexture(GL_TEXTURE0 + fp->Unit);
		glBindTexture(GL_TEXTURE_2D_ARRAY, ((FGLResourceTexture2DArray*)fp->Param2D)->textureHandle);
	);





	/*/
	SET_ENUM_SAMPLER_FUNC_PTR(TSampler3D,
		glActiveTexture(GL_TEXTURE0 + fp->Unit);
	glBindTexture(GL_TEXTURE_3D, ((FGLResourceTexture2D*)fp->Param3D)->textureHandle);
	);

	SET_ENUM_SAMPLER_FUNC_PTR(TSamplerCube,
		glActiveTexture(GL_TEXTURE0 + fp->Unit);
		glBindTexture(GL_TEXTURE_2D, ((FGLResourceTextureCubeMap*)fp->ParamCube)->textureHandle);
	);*/

	return true;
}

void OpenGLFRIDynamicAllocator::ClearBuffer(FResourceFrameBuffer* buffer, Color color)
{
	if (buffer)
	{
		BindFrameBuffer(buffer);
	}

	OpenGL::ClearDepth();
	OpenGL::ClearColor(color);

}

void OpenGLFRIDynamicAllocator::AttachLayeredTexture(FResourceTexture2DArray* tex)
{
	if (tex)
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, static_cast<FGLResourceTexture2DArray*>(tex)->textureHandle);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}
}

void OpenGLFRIDynamicAllocator::SetBackCull(bool b)
{
	if (b)
	{
		glCullFace(GL_BACK);
	}
	else
	{
		glCullFace(GL_FRONT);
	}
}



void OpenGLFRIDynamicAllocator::FlushMipMaps(FResourceTexture2D* tex)
{
	FGLResourceTexture2D* gltex = static_cast<FGLResourceTexture2D*>(tex);

	glBindTexture(gltex->textureType, gltex->textureHandle);
	glGenerateMipmap(gltex->textureType);
}
void OpenGLFRIDynamicAllocator::FlushMipMaps(FResourceTexture2DArray* tex)
{
	FGLResourceTexture2DArray* gltex = static_cast<FGLResourceTexture2DArray*>(tex);

	glBindTexture(gltex->textureType, gltex->textureHandle);
	glGenerateMipmap(gltex->textureType);
}