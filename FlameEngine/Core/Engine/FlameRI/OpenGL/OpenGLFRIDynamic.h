#pragma once


#include "../FRIDynamicAllocator.h"
#include "OpenGLFRIResource.h"

struct OpenGLFRIDynamicAllocator : FRIDynamicAllocator
{
public:
	FResourceIndexBuffer*		DynamicCreateIndexBuffer(uint32 Size, uint32 Usage, FResourceCreationDescriptor resourceDescriptor);
	FResourceVertexBuffer*		DynamicCreateVertexBuffer(uint32 Size, uint32 Usage, FResourceCreationDescriptor resourceDescriptor);


	FResourceVertexShader*		DynamicCreateVertexShader(const FArray<byte>& binCode);
	FResourcePixelShader*		DynamicCreatePixelShader(const FArray<byte>& binCode);
	FResourceGeometryShader*	DynamicCreateGeometryShader(const FArray<byte>& binCode);
	FResourceHullShader*		DynamicCreateHullShader(const FArray<byte>& binCode);
	FResourceDomainShader*		DynamicCreateDomainShader(const FArray<byte>& binCode);
	FResourceComputeShader*		DynamicCreateComputeShader(const FArray<byte>& binCode);

	FResourceShaderPipeline*	DynamicCreateShaderPipeline(FResourceShaderPipelineCreationDescriptor descriptor);
								
	FResourceTexture2D* DynamicCreateTexture2D(uint32 width, uint32 height, uint32 sampleCount, uint32 channels = 4, FResourceCreationDescriptor resourceDescriptor = FResourceCreationDescriptor(NULL, 0));
	FResourceTexture3D* DynamicCreateTexture3D(uint32 width, uint32 height, uint32 depth, FResourceCreationDescriptor resourceDescriptor);

	FResourceFrameBuffer*		DynamicCreateFrameBuffer();
};