#include "OpenGLFRI.h"
#include "FVXLToGLSL.h"
#include "OpenGLFRIProxyTypes.h"



typedef void (*_SamplerFuncPtr)(FUniformSampler* param);

FStaticArray<_SamplerFuncPtr, (size_t)EFRIUniformSamplerType::MaxSamplerTypes> _FSamplerFuncPtrs;

#define SET_ENUM_SAMPLER_FUNC_PTR(e, f) _FSamplerFuncPtrs[(size_t)EFRIUniformSamplerType::e]			= [](FUniformSampler* fp) { f }


FRIVertexBuffer* OpenGLFRIDynamicAllocator::CreateVertexBuffer(uint32 Size, uint32 Usage, FRICreationDescriptor resourceDescriptor)
{
	return new FGLResourceVertexBuffer(Size, Usage, resourceDescriptor);
}

FRIIndexBuffer* OpenGLFRIDynamicAllocator::CreateIndexBuffer(uint32 IndexCount, uint32 Usage, FRICreationDescriptor resourceDescriptor)
{
	return new FGLResourceIndexBuffer(IndexCount, Usage, resourceDescriptor);
}


FRITexture2D* OpenGLFRIDynamicAllocator::CreateTexture2D(uint32 width, uint32 height, uint32 sampleCount, EFRITextureFormat textureFormat, FRIColorDataFormat dataFormat, FRICreationDescriptor resourceDescriptor)
{

	GLuint gpuFormat = EGLFormatProxyEnum(textureFormat);
	GLuint channelFormat = EGLColorProxyEnum(dataFormat.channelFormat);
	GLuint dataPixelFormat = EGLPixelProxyEnum(dataFormat.pixelStorage);


	return new FGLResourceTexture2D(width, height, sampleCount, gpuFormat, channelFormat, dataPixelFormat, resourceDescriptor);
}

FRITexture3D* OpenGLFRIDynamicAllocator::CreateTexture3D(uint32 width, uint32 height, uint32 depth, FRICreationDescriptor resourceDescriptor)
{
	//FGLResourceTexture3D* fglTex2D = new FGLResourceTexture3D(width, height, sampleCount, GL_RGBA32F, 4, GL_FLOAT, resourceDescriptor);
	
	// TODO: implement

	return NULL;
}


FRITexture2DArray* OpenGLFRIDynamicAllocator::CreateTexture2DArray(uint32 width, uint32 height, uint32 numLayers, EFRITextureFormat textureFormat, FRIColorDataFormat dataFormat, FRICreationDescriptor resourceDescriptor)
{

	GLuint gpuFormat = EGLFormatProxyEnum(textureFormat);
	GLuint channelFormat = EGLColorProxyEnum(dataFormat.channelFormat);
	GLuint dataPixelFormat = EGLPixelProxyEnum(dataFormat.pixelStorage);

	return new FGLResourceTexture2DArray(width, height, numLayers, gpuFormat, channelFormat, dataPixelFormat, resourceDescriptor);
}




FRIShaderPipeline* OpenGLFRIDynamicAllocator::CreateShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor)
{
	return new FGLResourceShaderPipeline(descriptor);
}
FRIShaderPipeline* OpenGLFRIDynamicAllocator::CreateShaderPipeline(const ShaderLibraryModule& shaderModule)
{
	FArray<FRIShaderBase*> shaderArray;

	for (const auto& part : shaderModule.Parts)
	{
		FRIShaderBase* shaderResource;


		switch (part.Key)
		{
		case EFRIResourceShaderType::Vertex: shaderResource = CreateVertexShader(part.Value.Memory); break;
		case EFRIResourceShaderType::Pixel: shaderResource = CreatePixelShader(part.Value.Memory); break;
		case EFRIResourceShaderType::Geometry: shaderResource = CreateGeometryShader(part.Value.Memory); break;
		case EFRIResourceShaderType::Hull: shaderResource = CreateHullShader(part.Value.Memory); break;
		case EFRIResourceShaderType::Domain: shaderResource = CreateDomainShader(part.Value.Memory); break;

		}

		shaderArray.Add(shaderResource);
	}

	return new FGLResourceShaderPipeline(FRIShaderPipelineCreationDescriptor(shaderArray.Length(), shaderArray.Begin()));
}



FRIVertexShader* OpenGLFRIDynamicAllocator::CreateVertexShader(const FArray<byte>& binCode)
{
	return new FGLResourceVertexShader(binCode);
}
FRIPixelShader* OpenGLFRIDynamicAllocator::CreatePixelShader(const FArray<byte>& binCode)
{
	return new FGLResourcePixelShader(binCode);
}

FRIGeometryShader* OpenGLFRIDynamicAllocator::CreateGeometryShader(const FArray<byte>& binCode)
{
	return new FGLResourceGeometryShader(binCode);
}

FRIHullShader* OpenGLFRIDynamicAllocator::CreateHullShader(const FArray<byte>& binCode)
{
	return new FGLResourceHullShader(binCode);
}
FRIDomainShader* OpenGLFRIDynamicAllocator::CreateDomainShader(const FArray<byte>& binCode)
{
	return new FGLResourceDomainShader(binCode);
}
FRIComputeShader* OpenGLFRIDynamicAllocator::CreateComputeShader(const FArray<byte>& binCode)
{
	return new FGLResourceComputeShader(binCode);
}

FRIUniformBuffer* OpenGLFRIDynamicAllocator::CreateUniformBuffer(FRICreationDescriptor resource)
{
	return new FGLResourceUniformBuffer(resource);
}


FRIFrameBuffer* OpenGLFRIDynamicAllocator::CreateFrameBuffer(FArray<FRIFrameBufferAttachment> textureAttachments, bool enableDepthRenderBuffer)
{
	return new FGLResourceFrameBuffer(textureAttachments, enableDepthRenderBuffer);
}



FRIFrameBuffer* OpenGLFRIDynamicAllocator::CreateFrameBuffer(FRIFrameBufferArrayAttachment textureAttachments, bool enableDepthRenderBuffer)
{
	return NULL;// new FGLResourceFrameBuffer(textureAttachments, enableDepthRenderBuffer);
}


FRIVertexDeclaration* OpenGLFRIDynamicAllocator::CreateVertexDeclaration(FArray<FRIVertexDeclarationComponent> DeclCompArray, FRIVertexShader* shaderSignature)
{
	return new FRIVertexDeclaration(DeclCompArray);
}


FRIRasterizerState* OpenGLFRIDynamicAllocator::CreateRasterizerState(EFRICullMode cullMode, EFRIFillMode fillmode)
{
	return NULL;
}

FRIBlendState* OpenGLFRIDynamicAllocator::CreateBlendState(EFRIAlphaBlend srcBlend, EFRIAlphaBlend dstBlend)
{
	return NULL;
}



void OpenGLFRIDynamicAllocator::AttachVertexDeclaration(FRIVertexBuffer* geometry, FRIVertexDeclaration* declaration)
{
	FGLResourceVertexBuffer* glvb = static_cast<FGLResourceVertexBuffer*>(geometry);
	glBindVertexArray(glvb->vertexArrayBindingHandle);

	for (int i = 0; i < declaration->DeclarationElements.Length(); i++)
	{
		if (declaration->DeclarationElements[i].Type == EFRIVertexDeclerationAttributeType::Float)
		{
			glVertexAttribPointer
			(
				i,
				declaration->DeclarationElements[i].Length,
				(int32)declaration->DeclarationElements[i].Type,
				(GLboolean)declaration->DeclarationElements[i].Normalized,
				declaration->DeclarationElements[i].Stride,
				(void*)declaration->DeclarationElements[i].Offset
			);
		}
		else
		{
			glVertexAttribIPointer
			(
				i,
				declaration->DeclarationElements[i].Length,
				(int32)declaration->DeclarationElements[i].Type,
				declaration->DeclarationElements[i].Stride,
				(void*)declaration->DeclarationElements[i].Offset
			);
		}
		glEnableVertexAttribArray(i);
	}
}


void OpenGLFRIDynamicAllocator::BindFrameBuffer(FRIFrameBuffer* frameBuffer)
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

void OpenGLFRIDynamicAllocator::DrawPrimitivesIndexed(uint32 elementType, uint32 elementCount, uint32 indexType, FRIIndexBuffer* indexBuffer)
{	
	glDrawElements(elementType, indexBuffer->IndexCount, indexType, (void*)0);
}

void OpenGLFRIDynamicAllocator::SetShaderPipeline(FRIShaderPipeline* shader)
{
	FGLResourceShaderPipeline* glshader = (FGLResourceShaderPipeline*)shader;

	glUseProgram(glshader->pipelineHandle);
}
void OpenGLFRIDynamicAllocator::SetShaderUniformBuffer(uint32 slot, FRIUniformBuffer* uniformBuffer)
{
	FGLResourceUniformBuffer* glubuffer = static_cast<FGLResourceUniformBuffer*>(uniformBuffer);

	glBindBufferBase(GL_UNIFORM_BUFFER, slot, glubuffer->bufferHandle);

}

void OpenGLFRIDynamicAllocator::SetShaderSampler(FUniformSampler* sampler)
{
	(_FSamplerFuncPtrs[(size_t)sampler->samplerType])(sampler);
}

uint32 OpenGLFRIDynamicAllocator::GetShaderUniformParameterLocation(FRIShaderPipeline* shader, FAnsiString uniformName)
{
	return glGetUniformLocation(((FGLResourceShaderPipeline*)shader)->pipelineHandle, uniformName.ToPlatformString());
}



void OpenGLFRIDynamicAllocator::SetGeometrySource(FRIVertexBuffer* vertexBuffer)
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


void OpenGLFRIDynamicAllocator::SetTextureParameterBuffer(FRITexture2D* texture, FRITextureParameterBuffer paramBuffer)
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


void OpenGLFRIDynamicAllocator::SetTextureParameterBuffer(FRITexture2DArray* texture, FRITextureParameterBuffer paramBuffer)
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

void OpenGLFRIDynamicAllocator::SetFramebufferTextureLayer(FRIFrameBuffer* tex, uint32 layer)
{
	//glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, static_cast<FGLResourceTexture2DArray*>(tex)->textureHandle, 0, layer);
}



bool OpenGLFRIContext::InitializeOpenGLUniformExtensions()
{
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

void OpenGLFRIDynamicAllocator::ClearBuffer(FRIFrameBuffer* buffer, Color color)
{
	if (buffer)
	{
		BindFrameBuffer(buffer);
	}

	OpenGL::ClearDepth();
	OpenGL::ClearColor(color);

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



void OpenGLFRIDynamicAllocator::FlushMipMaps(FRITexture2D* tex)
{
	FGLResourceTexture2D* gltex = static_cast<FGLResourceTexture2D*>(tex);

	glBindTexture(gltex->textureType, gltex->textureHandle);
	glGenerateMipmap(gltex->textureType);
}
void OpenGLFRIDynamicAllocator::FlushMipMaps(FRITexture2DArray* tex)
{
	FGLResourceTexture2DArray* gltex = static_cast<FGLResourceTexture2DArray*>(tex);

	glBindTexture(gltex->textureType, gltex->textureHandle);
	glGenerateMipmap(gltex->textureType);
}

void OpenGLFRIDynamicAllocator::UniformBufferSubdata(FRIUniformBuffer* buffer, FRIUpdateDescriptor resource)
{
	FGLResourceUniformBuffer* fglubo = static_cast<FGLResourceUniformBuffer*>(buffer);

	glBindBuffer(GL_UNIFORM_BUFFER, fglubo->bufferHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, resource.Position, resource.ByteSize, resource.DataArray);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLFRIDynamicAllocator::StageResources(FRIResourceStageLambda stageLambda)
{
	stageLambda();
}

void OpenGLFRIDynamicAllocator::SetRasterizerState(FRIRasterizerState* rasterizer)
{

}

void OpenGLFRIDynamicAllocator::SetBlendState(FRIBlendState* blend)
{

}