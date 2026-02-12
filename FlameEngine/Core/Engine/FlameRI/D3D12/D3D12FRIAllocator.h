#pragma once

struct D3D12FRIDynamicAllocator : public FRIDynamicAllocator, public FD3D12DeviceChild
{
	friend class D3D12FRIContext;
public:

	D3D12FRIDynamicAllocator(FD3D12Device* Parent) :
		FD3D12DeviceChild(Parent)
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
		const ShaderLibraryModule& shaderModule,
		FRIInputLayout inputLayout,
		FArray<FRIRootParameter> RootParameters,
		FArray<FRIStaticSampler> StaticSamplers,
		FArray<EFRITextureFormat> RTVFormats,
		FRIRasterizerState RasterizerState,
		FRIDepthStencilState DepthStencilState,
		FRIBlendState BlendState
	);

	FRIPipelineStateObject* CreateComputePipelineStateObject(
		FArray<FRIRootParameter> RootParameters,
		const ShaderLibraryModule& shaderModule
	);
	//FRIRootSignature* CreateRootSignature();

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
	);
	FRIShaderResourceView* CreateShaderResourceView(
		FRITexture2DArray* target
	);
	FRIShaderResourceView* CreateShaderResourceView(
		FRITexture3D* target
	);

	FRIConstantBufferView* CreateConstantBufferView(
		FRIConstantBuffer* target
	);

	FRIDepthStencilView* CreateDepthStencilView(
		FRITexture2D* target
	);

	/* UAV */

	FRIUnorderedAccessView* CreateUnorderedAccessView(
		FRIConstantBuffer* target
	);

	FRIUnorderedAccessView* CreateUnorderedAccessView(
		FRIVertexBuffer * target
	);

	FRIUnorderedAccessView* CreateUnorderedAccessView(
		FRIIndexBuffer* target
	);

	FRIUnorderedAccessView* CreateUnorderedAccessView(
		FRIComputeBuffer* target
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
	);*/

	FRISamplerState* CreateSamplerState(
		EFRITextureFilter Filter,
		EFRITextureAddress AddressU,
		EFRITextureAddress AddressV,
		EFRITextureAddress AddressW,
		Color32 BorderColor = Color32()
	);

};