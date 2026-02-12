#pragma once


#include "../FRIDynamicAllocator.h"
#include "D3D11FRIResource.h"


class D3D11FRIContext;


struct D3D11FRIDynamicAllocator : FRIDynamicAllocator
{

	friend class D3D11FRIContext;
	D3D11FRIContext* D3DFRI;

public:

	D3D11FRIDynamicAllocator(D3D11FRIContext* context) :
		D3DFRI(context)
	{}

	FRIVertexShader* CreateVertexShader(const FArray<byte>& binCode);
	FRIPixelShader* CreatePixelShader(const FArray<byte>& binCode);
	FRIGeometryShader* CreateGeometryShader(const FArray<byte>& binCode);
	FRIHullShader* CreateHullShader(const FArray<byte>& binCode);
	FRIDomainShader* CreateDomainShader(const FArray<byte>& binCode);
	FRIComputeShader* CreateComputeShader(const FArray<byte>& binCode);


	FRIShaderPipeline* CreateShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor);
	FRIShaderPipeline* CreateShaderPipeline(const ShaderLibraryModule& shaderModule);


	FRIPipelineStateObject* CreatePipelineStateObject(
		FRIInputLayout inputLayout,
		FArray<FRIRootParameter> RootParameters,
		FArray<FRIStaticSampler> StaticSamplers,
		const ShaderLibraryModule& shaderModule,
		FArray<EFRITextureFormat> RTVFormats,
		uint32 NumRenderTargets
	);

	FRIPipelineStateObject* CreateComputePipelineStateObject(
		FArray<FRIRootParameter> RootParameters,
		const ShaderLibraryModule& shaderModule
	)
	{
		return 0;
	}


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
		FRICreationDescriptor resourceDescriptor = FRICreationDescriptor()
	);

	FRIComputeBuffer* CreateComputeBuffer(
		uint32 StructureCount,
		uint32 StructureStride,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor resourceDescriptor = FRICreationDescriptor()
	);

	FRIIndexBuffer* CreateIndexBuffer(
		uint32 IndexCount, 
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor resourceDescriptor = FRICreationDescriptor()
	);
	FRIVertexBuffer* CreateVertexBuffer(
		uint32 VertexCount,
		uint32 VertexSize,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor resourceDescriptor = FRICreationDescriptor()
	);
	FRIInstanceBuffer* CreateInstanceBuffer(
		uint32 InstanceCount, 
		uint32 InstanceStride, 
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor resourceDescriptor = FRICreationDescriptor()
	);


	FRIRenderTargetView* CreateRenderTargetView(
		FRITexture2D* target,
		uint32 targetMipLevel = 0
	);
	FRIRenderTargetView* CreateRenderTargetView(
		FRITexture2DArray* target,
		uint32 targetLayer,
		uint32 targetMipLayer = 0
	);
	FRIRenderTargetView* CreateRenderTargetView(
		FRITextureCubeMap* target,
		uint32 targetSide,
		uint32 targetMipLayer = 0
	);


	FRIShaderResourceView* CreateShaderResourceView(
		FRITexture2D* target
	) {
		return 0;
	};

	FRIShaderResourceView* CreateShaderResourceView(
		FRITexture2DArray* target
	) {
		return 0;
	};


	FRIConstantBufferView* CreateConstantBufferView(FRIConstantBuffer*)
	{
		return 0;
	}


	FRIDepthStencilView* CreateDepthStencilView(FRITexture2D*)
	{
		return 0;
	}

	FRIUnorderedAccessView* CreateUnorderedAccessView(
		FRIConstantBuffer* target
	) {
		return 0;
	};
	FRIUnorderedAccessView* CreateUnorderedAccessView(
		FRIVertexBuffer* target
	) {
		return 0;
	};

	FRIUnorderedAccessView* CreateUnorderedAccessView(
		FRIIndexBuffer* target
	) {
		return 0;
	};

	FRIUnorderedAccessView* CreateUnorderedAccessView(
		FRIComputeBuffer* target
	) {
		return 0;
	};


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
	/*
	FRIInputLayout* CreateInputLayout(
		FArray<FRIInputDesc> components, 
		FRIVertexShader* shaderSignature
	);*/

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
	void BindRenderTargets(FRIRenderTargetView* frameBuffer);
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

	void ClearRenderTarget(FRIRenderTargetView* buffer, Color32 color, FRIUnorderedAccessView** uavs, uint32 uavNum);

	void FlushMipMaps(FRIShaderResourceView* resource);

	void CopyResource(FRIResourceObject* source, FRIResourceObject* dest);
};