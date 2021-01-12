#include "OpenGLFRI.h"





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


FResourceTexture2D* OpenGLFRIDynamicAllocator::DynamicCreateTexture2D(uint32 width, uint32 height, uint32 sampleCount, uint32 channels = 4, FResourceCreationDescriptor resourceDescriptor = FResourceCreationDescriptor(NULL, 0))
{
	FGLResourceTexture2D* fglTex2D = new FGLResourceTexture2D(width, height, sampleCount, GL_RGBA32F, 4, GL_FLOAT, resourceDescriptor);

	return fglTex2D;
}

FResourceShaderPipeline* OpenGLFRIDynamicAllocator::DynamicCreateShaderPipeline(FResourceShaderPipelineCreationDescriptor descriptor)
{
	FGLResourceShaderPipeline* fglShaderPipeline = new FGLResourceShaderPipeline(descriptor);

	return fglShaderPipeline;
}


FResourceVertexShader* OpenGLFRIDynamicAllocator::DynamicCreateVertexShader(const FArray<byte>& binCode)
{
	return new FGLResourceVertexShader("VertexShader", binCode);
}
FResourcePixelShader* OpenGLFRIDynamicAllocator::DynamicCreatePixelShader(const FArray<byte>& binCode)
{
	return new FGLResourcePixelShader("PixelShader", binCode);
}

FResourceGeometryShader* OpenGLFRIDynamicAllocator::DynamicCreateGeometryShader(const FArray<byte>& binCode)
{
	return new FGLResourceGeometryShader("GeometryShader", binCode);
}

FResourceHullShader* OpenGLFRIDynamicAllocator::DynamicCreateHullShader(const FArray<byte>& binCode)
{
	return new FGLResourceHullShader("HullShader", binCode);
}
FResourceDomainShader* OpenGLFRIDynamicAllocator::DynamicCreateDomainShader(const FArray<byte>& binCode)
{
	return new FGLResourceDomainShader("HullShader", binCode);
}
FResourceComputeShader* OpenGLFRIDynamicAllocator::DynamicCreateComputeShader(const FArray<byte>& binCode)
{
	return new FGLResourceComputeShader("ComputeShader", binCode);
}

