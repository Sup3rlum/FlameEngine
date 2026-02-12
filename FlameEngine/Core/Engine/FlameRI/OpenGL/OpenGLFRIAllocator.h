#pragma once


#include "../FRIDynamicAllocator.h"
#include "OpenGLFRIResource.h"

struct OpenGLFRIDynamicAllocator : FRIDynamicAllocator
{
public:	
	
	FRIVertexShader* CreateVertexShader(const FArray<byte>& binCode);
	FRIPixelShader* CreatePixelShader(const FArray<byte>& binCode);
	FRIGeometryShader* CreateGeometryShader(const FArray<byte>& binCode);
	FRIHullShader* CreateHullShader(const FArray<byte>& binCode);
	FRIDomainShader* CreateDomainShader(const FArray<byte>& binCode);
	FRIComputeShader* CreateComputeShader(const FArray<byte>& binCode);


	FRIShaderPipeline* CreateShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor);
	FRIShaderPipeline* CreateShaderPipeline(const ShaderLibraryModule& shaderModule);

	/* Textures */

	FRITexture2D* CreateTexture2D(
		uint32 width,
		uint32 height,
		uint32 mipLevels,
		EFRIAccess Access,
		EFRITextureFormat format,
		FRICreationDescriptor resourceDescriptor[] = nullptr
	);

	FRITexture3D* CreateTexture3D(
		uint32 width,
		uint32 height,
		uint32 depth,
		uint32 mipLevels,
		EFRIAccess Access,
		EFRITextureFormat format,
		FRICreationDescriptor resourceDescriptor[] = nullptr
	);

	FRITexture2DArray* CreateTexture2DArray(
		uint32 width,
		uint32 height,
		uint32 numLayers,
		uint32 mipLevels,
		EFRIAccess Access,
		EFRITextureFormat format,
		FRICreationDescriptor resourceDescriptor[] = nullptr
	);

	FRITextureCubeMap* CreateTextureCubeMap(
		uint32 width,
		uint32 height,
		uint32 mipLevels,
		EFRIAccess Access,
		EFRITextureFormat format,
		FRICreationDescriptor resourceDescriptor[] = nullptr
	);

	/* Buffers */

	FRIConstantBuffer* CreateConstantBuffer(
		uint32 ByteSize,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor resourceDescriptor
	);

	FRIComputeBuffer* CreateComputeBuffer(
		uint32 ByteSize,
		uint32 StructureStride,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor resourceDescriptor
	);

	FRIIndexBuffer* CreateIndexBuffer(
		uint32 ByteSize,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor resourceDescriptor
	);
	FRIVertexBuffer* CreateVertexBuffer(
		uint32 ByteSize,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor resourceDescriptor
	);
	FRIInstanceBuffer* CreateInstanceBuffer(
		uint32 ByteSize,
		uint32 LayoutStride,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor resourceDescriptor
	);


	FRIRenderTargetView* CreateFrameBuffer(
		FArray<FRIFrameBufferAttachment> textureAttachments,
		bool enableDepthBuffer = true
	);

	FRIRenderTargetView* CreateFrameBuffer(
		FRIFrameBufferArrayAttachment textureAttachments,
		bool enableDepthBuffer = true,
		uint32 MipLevel = 0
	);

	/* States */

	FRIRasterizerState* CreateRasterizerState(
		EFRICullMode cullMode,
		EFRIFillMode fillmode
	);

	FRIBlendState* CreateBlendState(
		EFRIBlend srcBlend,
		EFRIBlend dstBlend,
		EFRIBlend srcAlphaBlend,
		EFRIBlend dstAlphaBlend
	);

	FRIDepthStencilState* CreateDepthStencilState(
		EFRIBool depthEnable,
		EFRIBool stencilEnable
	);

	/*FRIInputLayout* CreateInputLayout(
		FArray<FRIInputDesc> components,
		FRIVertexShader* shaderSignature
	);
	*/
	FRISamplerState* CreateSamplerState(
		EFRITextureFilter Filter,
		EFRITextureAddress AddressU,
		EFRITextureAddress AddressV,
		EFRITextureAddress AddressW,
		Color32 BorderColor = Color32()
	);


	void AttachInputLayout(FRIVertexBuffer* geometry, FRIInputLayout* declaration);
	void ResourceSubdata(FRIResourceObject* buffer, FRIUpdateDescriptor resource);


	void SetRasterizerState(FRIRasterizerState* rasterizer);
	void SetBlendState(FRIBlendState* blend);
	void SetDepthStencilState(FRIDepthStencilState* depth);

	void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);
	void BindFrameBuffer(FRIRenderTargetView* frameBuffer);
	void UnbindFrameBuffer();

	void SetGeometrySource(FRIVertexBuffer* vertexBuffer, FRIIndexBuffer* indexBuffer, FRIInstanceBuffer* instanceBuffer = nullptr);

	void SetShaderSamplerState(uint32 slot, FRISamplerState* sampler);
	void SetShaderResource(uint32 slot, FRIShaderResourceView* resource);

	void SetShaderPipeline(FRIShaderPipeline* shader);
	void SetShaderConstantBuffer(uint32 slot, FRIConstantBuffer* uniformBuffer, uint32 attachFlags = 63);

	/* Draw Calls */
	void DrawPrimitives(EFRIPrimitiveType primitveType, uint32 vertexCount);
	void DrawPrimitivesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, EFRIIndexType indexType, FRIIndexBuffer* indexBuffer);

	void DrawInstances(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount);
	void DrawInstancesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount, EFRIIndexType indexType, FRIIndexBuffer* indexBuffer);

	void DispatchCompute(
		uint32 xThreads,
		uint32 yThreads,
		uint32 zThreads
	);

	void StageResources(FRIConstantBuffer* ubo, FRIMemoryStageDelegate stageLambda);
	void SetUnorderedAccessView(uint32 slot, FRIUnorderedAccessView* buffer);

	void BeginScene();
	void BeginFrame();
	void EndScene();
	void EndFrame();

	void ClearBuffer(FRIRenderTargetView* buffer, Color32 color);
	void ClearBufferAndSetUnorderedAccessViews(FRIRenderTargetView* buffer, Color32 color, FRIUnorderedAccessView** uavs, uint32 uavNum);

	void FlushMipMaps(FRIShaderResourceView* resource);

	void CopyResource(FRIResourceObject* source, FRIResourceObject* dest);

};