#pragma once


#include "Core/Common/CoreCommon.h"
#include "FRIResource.h"

class FRIDynamicAllocator
{
public:
	virtual FResourceIndexBuffer* DynamicCreateIndexBuffer(uint32 IndexCount, uint32 Usage, FResourceCreationDescriptor resourceDescriptor);
	virtual FResourceVertexBuffer* DynamicCreateVertexBuffer(uint32 Size, uint32 Usage, FResourceCreationDescriptor resourceDescriptor);


	virtual FResourceVertexShader* DynamicCreateVertexShader(const FArray<byte>& binCode);
	virtual FResourcePixelShader* DynamicCreatePixelShader(const FArray<byte>& binCode);
	virtual FResourceGeometryShader* DynamicCreateGeometryShader(const FArray<byte>& binCode);
	virtual FResourceHullShader* DynamicCreateHullShader(const FArray<byte>& binCode);
	virtual FResourceDomainShader* DynamicCreateDomainShader(const FArray<byte>& binCode);
	virtual FResourceComputeShader* DynamicCreateComputeShader(const FArray<byte>& binCode);

	virtual FResourceShaderPipeline* DynamicCreateShaderPipeline(FResourceShaderPipelineCreationDescriptor descriptor);

	virtual FResourceTexture2D* DynamicCreateTexture2D(uint32 width, uint32 height, uint32 sampleCount, uint32 channels = 4, FResourceCreationDescriptor resourceDescriptor = FResourceCreationDescriptor(NULL, 0));
	virtual FResourceTexture3D* DynamicCreateTexture3D(uint32 width, uint32 height, uint32 depth, FResourceCreationDescriptor resourceDescriptor);
 
	virtual FResourceFrameBuffer* DynamicCreateFrameBuffer();


};