#include "OpenGLFRI.h"
#include "FVXLToGLSL.h"
#include "OpenGLFRIProxyTypes.h"


/*
typedef void (*_SamplerFuncPtr)(FRISampler param);

FStaticArray<_SamplerFuncPtr, (size_t)EFRISamplerType::MaxSamplerTypes> _FSamplerFuncPtrs;

#define SET_ENUM_SAMPLER_FUNC_PTR(e, f) _FSamplerFuncPtrs[(size_t)EFRISamplerType::e]			= [](FRISampler fp) { f }

*/

FRITexture2D* OpenGLFRIDynamicAllocator::CreateTexture2D(
	uint32 width,
	uint32 height,
	uint32 mipLevels,
	EFRIAccess Access,
	EFRITextureFormat format,
	FRICreationDescriptor resourceDescriptor[]
)
{
	/*
	GLuint gpuFormat = EGLFormatProxyEnum(textureFormat);
	GLuint channelFormat = EGLColorProxyEnum(dataFormat.channelFormat);
	GLuint dataPixelFormat = EGLPixelProxyEnum(dataFormat.pixelStorage);


	return new FGLResourceTexture2D(width, height, sampleCount, gpuFormat, channelFormat, dataPixelFormat, resourceDescriptor);*/

	return NULL;
}

FRITexture3D* OpenGLFRIDynamicAllocator::CreateTexture3D(
	uint32 width,
	uint32 height,
	uint32 depth,
	uint32 mipLevels,
	EFRIAccess Access,
	EFRITextureFormat format,
	FRICreationDescriptor resourceDescriptor[])
{
	//FGLResourceTexture3D* fglTex2D = new FGLResourceTexture3D(width, height, sampleCount, GL_RGBA32F, 4, GL_FLOAT, resourceDescriptor);
	
	// TODO: implement

	return NULL;
}


FRITexture2DArray* OpenGLFRIDynamicAllocator::CreateTexture2DArray(
	uint32 width,
	uint32 height,
	uint32 numLayers,
	uint32 mipLevels,
	EFRIAccess Access,
	EFRITextureFormat format,
	FRICreationDescriptor resourceDescriptor[])
{
	/*
	GLuint gpuFormat = EGLFormatProxyEnum(textureFormat);
	GLuint channelFormat = EGLColorProxyEnum(dataFormat.channelFormat);
	GLuint dataPixelFormat = EGLPixelProxyEnum(dataFormat.pixelStorage);
	*/
	return NULL;// new FGLResourceTexture2DArray(width, height, numLayers, gpuFormat, channelFormat, dataPixelFormat, resourceDescriptor);
}




FRIShaderPipeline* OpenGLFRIDynamicAllocator::CreateShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor)
{
	return NULL;// new FGLResourceShaderPipeline(descriptor);
}
FRIShaderPipeline* OpenGLFRIDynamicAllocator::CreateShaderPipeline(const ShaderLibraryModule& shaderModule)
{
	FArray<FRIShaderBase*> shaderArray;

	for (const auto& part : shaderModule.Parts)
	{
		FRIShaderBase* shaderResource;


		switch (part.Key)
		{
		case EFRIShaderType::Vertex: shaderResource = CreateVertexShader(part.Value.Memory); break;
		case EFRIShaderType::Pixel: shaderResource = CreatePixelShader(part.Value.Memory); break;
		case EFRIShaderType::Geometry: shaderResource = CreateGeometryShader(part.Value.Memory); break;
		case EFRIShaderType::Hull: shaderResource = CreateHullShader(part.Value.Memory); break;
		case EFRIShaderType::Domain: shaderResource = CreateDomainShader(part.Value.Memory); break;

		}

		shaderArray.Add(shaderResource);
	}

	return NULL;// new FGLResourceShaderPipeline(FRIShaderPipelineCreationDescriptor(shaderArray.Length(), shaderArray.Begin()));
}



FRIVertexShader* OpenGLFRIDynamicAllocator::CreateVertexShader(const FArray<byte>& binCode)
{
	return NULL;// new FGLResourceVertexShader(binCode);
}
FRIPixelShader* OpenGLFRIDynamicAllocator::CreatePixelShader(const FArray<byte>& binCode)
{
	return NULL;// new FGLResourcePixelShader(binCode);
}

FRIGeometryShader* OpenGLFRIDynamicAllocator::CreateGeometryShader(const FArray<byte>& binCode)
{
	return NULL;//new FGLResourceGeometryShader(binCode);
}

FRIHullShader* OpenGLFRIDynamicAllocator::CreateHullShader(const FArray<byte>& binCode)
{
	return NULL;//new FGLResourceHullShader(binCode);
}
FRIDomainShader* OpenGLFRIDynamicAllocator::CreateDomainShader(const FArray<byte>& binCode)
{
	return NULL;//new FGLResourceDomainShader(binCode);
}
FRIComputeShader* OpenGLFRIDynamicAllocator::CreateComputeShader(const FArray<byte>& binCode)
{
	return NULL;//new FGLResourceComputeShader(binCode);
}


FRIVertexBuffer* OpenGLFRIDynamicAllocator::CreateVertexBuffer(
	uint32 Size,
	EFRIAccess Access,
	EFRIUsage Usage,
	FRICreationDescriptor resourceDescriptor)
{
	return NULL;// new FGLResourceVertexBuffer(Size, Usage, resourceDescriptor);
}

FRIIndexBuffer* OpenGLFRIDynamicAllocator::CreateIndexBuffer(
	uint32 Size,
	EFRIAccess Access,
	EFRIUsage Usage,
	FRICreationDescriptor resourceDescriptor)
{
	return NULL;// new FGLResourceIndexBuffer(IndexCount, Usage, resourceDescriptor);
}


FRIInstanceBuffer* OpenGLFRIDynamicAllocator::CreateInstanceBuffer(
	uint32 Size,
	uint32 LayoutStride,
	EFRIAccess Access,
	EFRIUsage Usage,
	FRICreationDescriptor resourceDescriptor)
{
	return NULL;
}

FRIConstantBuffer* OpenGLFRIDynamicAllocator::CreateConstantBuffer(
	uint32 ByteSize,
	EFRIAccess Access,
	EFRIUsage Usage,
	FRICreationDescriptor resourceDescriptor)
{
	return NULL;// new FGLResourceConstantBuffer(resource);
}





FRIComputeBuffer* OpenGLFRIDynamicAllocator::CreateComputeBuffer(
	uint32 ByteSize,
	uint32 StructureStride,
	EFRIAccess Access,
	EFRIUsage Usage,
	FRICreationDescriptor resourceDescriptor)
{
	return  NULL;
}


FRIRenderTargetView* OpenGLFRIDynamicAllocator::CreateFrameBuffer(FArray<FRIFrameBufferAttachment> textureAttachments, bool enableDepthRenderBuffer)
{
	return NULL;// new FGLResourceFrameBuffer(textureAttachments, enableDepthRenderBuffer);
}



FRIRenderTargetView* OpenGLFRIDynamicAllocator::CreateFrameBuffer(FRIFrameBufferArrayAttachment textureAttachments, bool enableDepthRenderBuffer, uint32 mipLevel)
{
	return NULL;// new FGLResourceFrameBuffer(textureAttachments, enableDepthRenderBuffer);
}
/*
FRIInputLayout* OpenGLFRIDynamicAllocator::CreateInputLayout(FArray<FRIInputDesc> DeclCompArray, FRIVertexShader* shaderSignature)
{
	return NULL;
}
*/

FRIRasterizerState* OpenGLFRIDynamicAllocator::CreateRasterizerState(EFRICullMode cullMode, EFRIFillMode fillmode)
{
	return NULL;
}

FRIBlendState* OpenGLFRIDynamicAllocator::CreateBlendState(EFRIBlend srcBlend, EFRIBlend dstBlend, EFRIBlend srcAlphaBlend, EFRIBlend dstAlphaBlend)
{
	return NULL;
}


FRIDepthStencilState* OpenGLFRIDynamicAllocator::CreateDepthStencilState(EFRIBool depth, EFRIBool stencil)
{
	return NULL;
}


FRISamplerState* OpenGLFRIDynamicAllocator::CreateSamplerState(
	EFRITextureFilter Filter,
	EFRITextureAddress AddressU,
	EFRITextureAddress AddressV,
	EFRITextureAddress AddressW,
	Color32 BorderColor)
{
	return NULL;
}

void OpenGLFRIDynamicAllocator::SetShaderResource(uint32 slot, FRIShaderResourceView* resource)
{

}

void OpenGLFRIDynamicAllocator::AttachInputLayout(FRIVertexBuffer* geometry, FRIInputLayout* declaration)
{
	/*
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
	}*/
}


void OpenGLFRIDynamicAllocator::BindFrameBuffer(FRIRenderTargetView* frameBuffer)
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
void OpenGLFRIDynamicAllocator::DrawPrimitives(EFRIPrimitiveType primitveType, uint32 vertexCount)
{
	glDrawArrays((uint32)primitveType, 0, vertexCount);
}

void OpenGLFRIDynamicAllocator::DrawPrimitivesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, EFRIIndexType indexType, FRIIndexBuffer* indexBuffer)
{	
	glDrawElements((uint32)primitveType, vertexCount, (uint32)indexType, (void*)0);
}


void OpenGLFRIDynamicAllocator::DrawInstances(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount)
{

}

void OpenGLFRIDynamicAllocator::DrawInstancesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount, EFRIIndexType indexType, FRIIndexBuffer* indexBuffer)
{

}





void OpenGLFRIDynamicAllocator::SetShaderPipeline(FRIShaderPipeline* shader)
{
	FGLResourceShaderPipeline* glshader = (FGLResourceShaderPipeline*)shader;

	glUseProgram(glshader->pipelineHandle);
}
void OpenGLFRIDynamicAllocator::SetShaderConstantBuffer(uint32 slot, FRIConstantBuffer* uniformBuffer, uint32)
{
	//FGLResourceConstantBuffer* glubuffer = static_cast<FGLResourceConstantBuffer*>(uniformBuffer);

	//glBindBufferBase(GL_UNIFORM_BUFFER, slot, glubuffer->bufferHandle);

}

void OpenGLFRIDynamicAllocator::SetShaderSamplerState(uint32 slot, FRISamplerState* sampler)
{
	//(_FSamplerFuncPtrs[(size_t)sampler.samplerType])(sampler);
}


void OpenGLFRIDynamicAllocator::SetGeometrySource(FRIVertexBuffer* vertexBuffer, FRIIndexBuffer* indexBuffer, FRIInstanceBuffer* instanceBuffer)
{
	//glBindVertexArray(((FGLResourceVertexBuffer*)vertexBuffer)->vertexArrayBindingHandle);
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

/*
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


}*/

/*
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
}*/



bool OpenGLFRIContext::InitializeOpenGLUniformExtensions()
{
	/*
	SET_ENUM_SAMPLER_FUNC_PTR(TSampler2D,

		glActiveTexture(GL_TEXTURE0 + fp.Unit);
		glBindTexture(GL_TEXTURE_2D, ((FGLResourceTexture2D*)fp.Param2D)->textureHandle);
		);


	SET_ENUM_SAMPLER_FUNC_PTR(TSampler2DArray,

		glActiveTexture(GL_TEXTURE0 + fp.Unit);
		glBindTexture(GL_TEXTURE_2D_ARRAY, ((FGLResourceTexture2DArray*)fp.Param2D)->textureHandle);
	);





	/
	SET_ENUM_SAMPLER_FUNC_PTR(TSampler3D,
		glActiveTexture(GL_TEXTURE0 + fp->Unit);
	glBindTexture(GL_TEXTURE_3D, ((FGLResourceTexture2D*)fp->Param3D)->textureHandle);
	);

	SET_ENUM_SAMPLER_FUNC_PTR(TSamplerCube,
		glActiveTexture(GL_TEXTURE0 + fp->Unit);
		glBindTexture(GL_TEXTURE_2D, ((FGLResourceTextureCubeMap*)fp->ParamCube)->textureHandle);
	);

	return true;*/

	return true;
}

void OpenGLFRIDynamicAllocator::ClearBuffer(FRIRenderTargetView* buffer, Color32 color)
{
	if (buffer)
	{
		BindFrameBuffer(buffer);
	}

	OpenGL::ClearDepth();
	OpenGL::ClearColor(color);

}


void OpenGLFRIDynamicAllocator::FlushMipMaps(FRIShaderResourceView* tex)
{
	/*
	FGLResourceTexture2D* gltex = static_cast<FGLResourceTexture2D*>(tex);

	glBindTexture(gltex->textureType, gltex->textureHandle);
	glGenerateMipmap(gltex->textureType);*/
}

void OpenGLFRIDynamicAllocator::ResourceSubdata(FRIResourceObject* buffer, FRIUpdateDescriptor resource)
{
	/*
	FGLResourceConstantBuffer* fglubo = static_cast<FGLResourceConstantBuffer*>(buffer);

	glBindBuffer(GL_UNIFORM_BUFFER, fglubo->bufferHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, resource.Position, resource.ByteSize, resource.DataArray);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);*/
}


void OpenGLFRIDynamicAllocator::StageResources(FRIConstantBuffer* ubo, FRIMemoryStageDelegate stageLambda)
{

}

void OpenGLFRIDynamicAllocator::SetRasterizerState(FRIRasterizerState* rasterizer)
{

}

void OpenGLFRIDynamicAllocator::SetBlendState(FRIBlendState* blend)
{

}
void OpenGLFRIDynamicAllocator::SetDepthStencilState(FRIDepthStencilState* depth)
{

}
void OpenGLFRIDynamicAllocator::CopyResource(FRIResourceObject* source, FRIResourceObject* dest)
{


}

void OpenGLFRIDynamicAllocator::SetUnorderedAccessView(uint32 slot, FRIUnorderedAccessView* tex)
{
}


void OpenGLFRIDynamicAllocator::DispatchCompute(uint32 x, uint32 y, uint32 z)
{

}

FRITextureCubeMap* OpenGLFRIDynamicAllocator::CreateTextureCubeMap(uint32 width, uint32 height, uint32 mipLevels, EFRIAccess access, EFRITextureFormat format, FRICreationDescriptor resourceDescriptor[])
{
	return NULL;
}