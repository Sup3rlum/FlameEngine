#pragma once


#include "Core/Common/CoreCommon.h"
#include "FRIResource.h"
#include "ShaderLibrary/ShaderLibrary.h"


class FRIDynamicAllocator
{
public:

	virtual FRIVertexShader* CreateVertexShader(const FArray<byte>& binCode) = 0;
	virtual FRIPixelShader* CreatePixelShader(const FArray<byte>& binCode) = 0;
	virtual FRIGeometryShader* CreateGeometryShader(const FArray<byte>& binCode) = 0;
	virtual FRIHullShader* CreateHullShader(const FArray<byte>& binCode) = 0;
	virtual FRIDomainShader* CreateDomainShader(const FArray<byte>& binCode) = 0;
	virtual FRIComputeShader* CreateComputeShader(const FArray<byte>& binCode) = 0;

	virtual FRIShaderPipeline* CreateShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor) = 0;
	virtual FRIShaderPipeline* CreateShaderPipeline(const ShaderLibraryModule& shaderModule) = 0;

	virtual FRIPipelineStateObject* CreatePipelineStateObject(
		const ShaderLibraryModule& shaderModule,
		FRIInputLayout inputLayout,
		FArray<FRIRootParameter> RootParameters,
		FArray<FRIStaticSampler> StaticSamplers,
		FArray<EFRITextureFormat> RTVFormats,
		FRIRasterizerState RasterizerState,
		FRIDepthStencilState DepthStencilState,
		FRIBlendState BlendState
	) = 0;

	virtual FRIPipelineStateObject* CreateComputePipelineStateObject(
		FArray<FRIRootParameter> RootParameters,
		const ShaderLibraryModule& shaderModule
	) = 0;

	/* Textures */

	virtual FRITexture2D* CreateTexture2D(
		uint32 width,
		uint32 height,
		uint32 mipLevels,
		EFRIAccess Access,
		EFRITextureFormat format,
		FRICreationDescriptor resourceDescriptor[] = nullptr
	) = 0;

	virtual FRITexture3D* CreateTexture3D(
		uint32 width,
		uint32 height,
		uint32 depth,
		uint32 mipLevels,
		EFRIAccess Access,
		EFRITextureFormat format,
		FRICreationDescriptor resourceDescriptor[] = nullptr
	) = 0;

	virtual FRITexture2DArray* CreateTexture2DArray(
		uint32 width,
		uint32 height,
		uint32 numLayers,
		uint32 mipLevels,
		EFRIAccess Access,
		EFRITextureFormat format,
		FRICreationDescriptor resourceDescriptor[] = nullptr
	) = 0;

	virtual FRITextureCubeMap* CreateTextureCubeMap(
		uint32 width,
		uint32 height,
		uint32 mipLevels,
		EFRIAccess Access,
		EFRITextureFormat format,
		FRICreationDescriptor resourceDescriptor[] = nullptr
	) = 0;

	/* Buffers */

	virtual FRIConstantBuffer* CreateConstantBuffer(
		uint32 ByteSize,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor resourceDescriptor = FRICreationDescriptor()
	) = 0;

	virtual FRIComputeBuffer* CreateComputeBuffer(
		uint32 StructureCount,
		uint32 StructureStride,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor resourceDescriptor = FRICreationDescriptor()
	) = 0;

	virtual FRIIndexBuffer* CreateIndexBuffer(
		uint32 IndexCount,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor resourceDescriptor = FRICreationDescriptor()
	) = 0;
	virtual FRIVertexBuffer* CreateVertexBuffer(
		uint32 VertexCount,
		uint32 VertexSize,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor resourceDescriptor = FRICreationDescriptor()
	) = 0;
	virtual FRIInstanceBuffer* CreateInstanceBuffer(
		uint32 InstanceCount,
		uint32 InstanceStride,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor resourceDescriptor = FRICreationDescriptor()
	) = 0;


	virtual FRIRenderTargetView* CreateRenderTargetView(
		FRITexture2D* target,
		uint32 targetMipLevel = 0
	) = 0;
	virtual FRIRenderTargetView* CreateRenderTargetView(
		FRITexture2DArray* target,
		uint32 targetLayer,
		uint32 targetMipLayer = 0
	) = 0;
	virtual FRIRenderTargetView* CreateRenderTargetView(
		FRITextureCubeMap* target,
		uint32 targetSide,
		uint32 targetMipLayer = 0
	) = 0;

	//SRV
	virtual FRIShaderResourceView* CreateShaderResourceView(
		FRITexture2D* target
	) = 0;
	virtual FRIShaderResourceView* CreateShaderResourceView(
		FRITexture2DArray* target
	) = 0;
	virtual FRIShaderResourceView* CreateShaderResourceView(
		FRITexture3D* target
	) = 0;

	// CBV
	virtual FRIConstantBufferView* CreateConstantBufferView(
		FRIConstantBuffer* target
	) = 0;

	// UAV
	virtual FRIUnorderedAccessView* CreateUnorderedAccessView(
		FRIConstantBuffer* target
	) = 0;

	virtual FRIUnorderedAccessView* CreateUnorderedAccessView(
		FRIVertexBuffer* target
	) = 0;

	virtual FRIUnorderedAccessView* CreateUnorderedAccessView(
		FRIIndexBuffer* target
	) = 0;

	virtual FRIUnorderedAccessView* CreateUnorderedAccessView(
		FRIComputeBuffer* target
	) = 0;


	// DSV

	virtual FRIDepthStencilView* CreateDepthStencilView(
		FRITexture2D* target
	) = 0;

	/* States */

	virtual FRIRasterizerState* CreateRasterizerState(
		EFRICullMode cullMode,
		EFRIFillMode fillmode
	) = 0;

	virtual FRIBlendState* CreateBlendState(
		EFRIBlend srcBlend,
		EFRIBlend dstBlend,
		EFRIBlend srcAlphaBlend,
		EFRIBlend dstAlphaBlend
	) = 0;

	virtual FRIDepthStencilState* CreateDepthStencilState(
		EFRIBool depthEnable,
		EFRIBool stencilEnable
	) = 0;

	/*virtual FRIInputLayout* CreateInputLayout(
		FArray<FRIInputDesc> components,
		FRIVertexShader* shaderSignature
	) = 0;*/

	virtual FRISamplerState* CreateSamplerState(
		EFRITextureFilter Filter,
		EFRITextureAddress AddressU,
		EFRITextureAddress AddressV,
		EFRITextureAddress AddressW,
		Color32 BorderColor = Color32()
	) = 0;
};