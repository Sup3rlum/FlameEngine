#pragma once


#include "Core/Common/CoreCommon.h"
#include "FRIResource.h"
#include "ShaderLibrary/ShaderLibrary.h"




class FRIDynamicAllocator
{
public:
	virtual FRIIndexBuffer* CreateIndexBuffer(uint32 IndexCount, uint32 Usage, FRICreationDescriptor resourceDescriptor) = 0;
	virtual FRIVertexBuffer* CreateVertexBuffer(uint32 Size, uint32 Usage, FRICreationDescriptor resourceDescriptor) = 0;
	virtual FRIInstanceBuffer* CreateInstanceBuffer(uint32 Size, uint32 LayoutStride, FRICreationDescriptor resourceDescriptor) = 0;


	virtual FRIVertexShader*		CreateVertexShader(const FArray<byte>& binCode) = 0;
	virtual FRIPixelShader*		CreatePixelShader(const FArray<byte>& binCode) = 0;
	virtual FRIGeometryShader*	CreateGeometryShader(const FArray<byte>& binCode) = 0;
	virtual FRIHullShader*		CreateHullShader(const FArray<byte>& binCode) = 0;
	virtual FRIDomainShader*		CreateDomainShader(const FArray<byte>& binCode) = 0;
	virtual FRIComputeShader*		CreateComputeShader(const FArray<byte>& binCode) = 0;



	virtual FRIShaderPipeline* CreateShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor) = 0;
	virtual FRIShaderPipeline* CreateShaderPipeline(const ShaderLibraryModule& shaderModule) = 0;

	virtual FRITexture2D* CreateTexture2D(uint32 width, uint32 height, uint32 sampleCount, EFRITextureFormat format, FRIColorDataFormat colorData = FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::Byte), FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0), bool cpuWrite = false) = 0;
	virtual FRITexture3D* CreateTexture3D(uint32 width, uint32 height, uint32 depth, EFRITextureFormat format, FRIColorDataFormat colorData = FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::Byte), FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0), bool cpuWrite = false, bool render = true) = 0;
	
	virtual FRITexture2DArray* CreateTexture2DArray(uint32 width, uint32 height, uint32 numLayers, EFRITextureFormat format, FRIColorDataFormat colorData = FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::Byte), FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0)) = 0;
	virtual FRIFrameBuffer* CreateFrameBuffer(FArray<FRIFrameBufferAttachment> textureAttachments, bool enableDepthRenderBuffer = true) = 0;
	virtual FRIFrameBuffer* CreateFrameBuffer(FRIFrameBufferArrayAttachment textureAttachment, bool enableDepthRenderBuffer = true) = 0;
	virtual FRIUniformBuffer* CreateUniformBuffer(FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0)) = 0;

	virtual FRIRasterizerState* CreateRasterizerState(EFRICullMode cullMode, EFRIFillMode fillmode) = 0;
	virtual FRIBlendState* CreateBlendState(EFRIAlphaBlend srcBlend, EFRIAlphaBlend dstBlend) = 0;
	virtual FRIDepthStencilState* CreateDepthStencilState(EFRIBool depthEnable, EFRIBool stencilEnable) = 0;

	virtual FRIVertexDeclaration* CreateVertexDeclaration(FArray<FRIVertexDeclarationDesc> components, FRIVertexShader* shaderSignature) = 0;
	virtual void AttachVertexDeclaration(FRIVertexBuffer* geometry, FRIVertexDeclaration* declaration) = 0;

	virtual void UniformBufferSubdata(FRIUniformBuffer* buffer, FRIUpdateDescriptor resource) = 0;
	virtual void InstanceBufferSubdata(FRIInstanceBuffer* buffer, FRIUpdateDescriptor resource) = 0;
	virtual void Texture2DSubdata(FRITexture2D* texture, FRIUpdateDescriptor resource) = 0;
	virtual void Texture3DSubdata(FRITexture3D* texture, FRIUpdateDescriptor resource) = 0;
	virtual void VertexBufferSubdata(FRIVertexBuffer* buffer, FRIUpdateDescriptor resource) = 0;

	virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
	virtual void BindFrameBuffer(FRIFrameBuffer* frameBuffer) = 0;
	virtual void UnbindFrameBuffer() = 0;

	virtual void SetGeometrySource(FRIVertexBuffer* vertexBuffer) = 0;
	virtual void SetInstancedGeometrySource(FRIVertexBuffer* vertexBuffer, FRIInstanceBuffer* instanceBuffer) = 0;

	virtual void DrawPrimitives(EFRIPrimitiveType primitveType, uint32 vertexCount) = 0;
	virtual void DrawPrimitivesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, EFRIIndexType indexType, FRIIndexBuffer* indexBuffer) = 0;


	virtual void DrawInstances(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount) = 0;
	virtual void DrawInstancesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount, EFRIIndexType indexType, FRIIndexBuffer* indexBuffer) = 0;

	virtual void SetShaderPipeline(FRIShaderPipeline* shader) = 0;
	virtual void SetShaderUniformBuffer(uint32 slot, FRIUniformBuffer* uniformBuffer) = 0;
	virtual void SetShaderSampler(FUniformSampler* sampler) = 0;
	virtual void SetTextureParameterBuffer(FRITexture2D* texture, FRITextureParameterBuffer param) = 0;
	virtual void SetTextureParameterBuffer(FRITexture2DArray* texture, FRITextureParameterBuffer param) = 0;

	virtual void SetFramebufferTextureLayer(FRIFrameBuffer* frameBuffer, uint32 layer) = 0;

	virtual void StageResources(FRIUniformBuffer* ubo, FRIMemoryStageDelegate stageLambda) = 0;

	virtual void SetRasterizerState(FRIRasterizerState* rasterizer) = 0;
	virtual void SetBlendState(FRIBlendState* blend) = 0;
	virtual void SetDepthStencilState(FRIDepthStencilState* depth) = 0;

	virtual void SetUAV(uint32 slot, FRITexture3D* tex) = 0;

	virtual void BeginScene() = 0;
	virtual void BeginFrame() = 0;
	virtual void EndScene() = 0;
	virtual void EndFrame() = 0;

	virtual void ClearBuffer(FRIFrameBuffer* buffer, Color color) = 0;
	virtual void ClearBufferAndSetUAVs(FRIFrameBuffer* buffer, Color color, FRITexture3D** uavs, uint32 slots) = 0;

	virtual void FlushMipMaps(FRITexture2D* tex) = 0;
	virtual void FlushMipMaps(FRITexture2DArray* tex) = 0;
	virtual void FlushMipMaps(FRITexture3D* tex) = 0;

	virtual void CopyTexture(FRITexture2D* source, FRITexture2D* dest) = 0;

	virtual void DispatchCompute(uint32 xThreads, uint32 yThreads, uint32 zThreads) = 0;

};