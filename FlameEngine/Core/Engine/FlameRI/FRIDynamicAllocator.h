#pragma once


#include "Core/Common/CoreCommon.h"
#include "FRIResource.h"
#include "ShaderLibrary/ShaderLibrary.h"




class FRIDynamicAllocator
{
public:
	virtual FRIIndexBuffer* CreateIndexBuffer(uint32 IndexCount, uint32 Usage, FRICreationDescriptor resourceDescriptor) = 0;
	virtual FRIVertexBuffer* CreateVertexBuffer(uint32 Size, uint32 Usage, FRICreationDescriptor resourceDescriptor) = 0;


	virtual FRIVertexShader*		CreateVertexShader(const FArray<byte>& binCode) = 0;
	virtual FRIPixelShader*		CreatePixelShader(const FArray<byte>& binCode) = 0;
	virtual FRIGeometryShader*	CreateGeometryShader(const FArray<byte>& binCode) = 0;
	virtual FRIHullShader*		CreateHullShader(const FArray<byte>& binCode) = 0;
	virtual FRIDomainShader*		CreateDomainShader(const FArray<byte>& binCode) = 0;
	virtual FRIComputeShader*		CreateComputeShader(const FArray<byte>& binCode) = 0;



	virtual FRIShaderPipeline* CreateShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor) = 0;
	virtual FRIShaderPipeline* CreateShaderPipeline(const ShaderLibraryModule& shaderModule) = 0;

	virtual FRITexture2D* CreateTexture2D(uint32 width, uint32 height, uint32 sampleCount, EFRITextureFormat format, FRIColorDataFormat colorData = FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::Byte), FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0)) = 0;
	virtual FRITexture3D* CreateTexture3D(uint32 width, uint32 height, uint32 depth, FRICreationDescriptor resourceDescriptor) = 0;
	
	virtual FRITexture2DArray* CreateTexture2DArray(uint32 width, uint32 height, uint32 numLayers, EFRITextureFormat format, FRIColorDataFormat colorData = FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::Byte), FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0)) = 0;
	virtual FRIFrameBuffer* CreateFrameBuffer(FArray<FRIFrameBufferAttachment> textureAttachments, bool enableDepthRenderBuffer = true) = 0;
	virtual FRIFrameBuffer* CreateFrameBuffer(FRIFrameBufferArrayAttachment textureAttachment, bool enableDepthRenderBuffer = true) = 0;
	virtual FRIUniformBuffer* CreateUniformBuffer(FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0)) = 0;

	virtual FRIRasterizerState* CreateRasterizerState(EFRICullMode cullMode, EFRIFillMode fillmode) = 0;
	virtual FRIBlendState* CreateBlendState(EFRIAlphaBlend srcBlend, EFRIAlphaBlend dstBlend) = 0;

	virtual FRIVertexDeclaration* CreateVertexDeclaration(FArray<FRIVertexDeclarationComponent> components, FRIVertexShader* shaderSignature) = 0;
	virtual void AttachVertexDeclaration(FRIVertexBuffer* geometry, FRIVertexDeclaration* declaration) = 0;

	virtual void UniformBufferSubdata(FRIUniformBuffer* buffer, FRIUpdateDescriptor resource) = 0;


	virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
	virtual void BindFrameBuffer(FRIFrameBuffer* frameBuffer) = 0;
	virtual void UnbindFrameBuffer() = 0;

	virtual void SetGeometrySource(FRIVertexBuffer* vertexBuffer) = 0;
	virtual void DrawPrimitives(uint32 elementType, uint32 elementCount) = 0;
	virtual void DrawPrimitivesIndexed(uint32 elementType, uint32 elementCount, uint32 indexType, FRIIndexBuffer* indexBuffer) = 0;

	virtual void SetShaderPipeline(FRIShaderPipeline* shader) = 0;
	virtual void SetShaderUniformBuffer(uint32 slot, FRIUniformBuffer* uniformBuffer) = 0;
	virtual void SetShaderSampler(FUniformSampler* sampler) = 0;
	virtual void SetTextureParameterBuffer(FRITexture2D* texture, FRITextureParameterBuffer param) = 0;
	virtual void SetTextureParameterBuffer(FRITexture2DArray* texture, FRITextureParameterBuffer param) = 0;

	virtual void SetFramebufferTextureLayer(FRIFrameBuffer* frameBuffer, uint32 layer) = 0;

	virtual void StageResources(FRIResourceStageLambda stageLambda) = 0;

	virtual void SetRasterizerState(FRIRasterizerState* rasterizer) = 0;
	virtual void SetBlendState(FRIBlendState* blend) = 0;

	virtual void BeginScene() = 0;
	virtual void BeginFrame() = 0;
	virtual void EndScene() = 0;
	virtual void EndFrame() = 0;

	virtual void ClearBuffer(FRIFrameBuffer* buffer, Color color) = 0;

	virtual void FlushMipMaps(FRITexture2D* tex) = 0;
	virtual void FlushMipMaps(FRITexture2DArray* tex) = 0;

};