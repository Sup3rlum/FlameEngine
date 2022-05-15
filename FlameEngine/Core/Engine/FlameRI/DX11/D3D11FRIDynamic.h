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

	FRIIndexBuffer* CreateIndexBuffer(uint32 Size, uint32 Usage, FRICreationDescriptor resourceDescriptor);
	FRIVertexBuffer* CreateVertexBuffer(uint32 Size, uint32 Usage, FRICreationDescriptor resourceDescriptor);
	FRIInstanceBuffer* CreateInstanceBuffer(uint32 Size, uint32 LayoutStride, FRICreationDescriptor resourceDescriptor);

	FRIVertexShader* CreateVertexShader(const FArray<byte>& binCode);
	FRIPixelShader* CreatePixelShader(const FArray<byte>& binCode);
	FRIGeometryShader* CreateGeometryShader(const FArray<byte>& binCode);
	FRIHullShader* CreateHullShader(const FArray<byte>& binCode);
	FRIDomainShader* CreateDomainShader(const FArray<byte>& binCode);
	FRIComputeShader* CreateComputeShader(const FArray<byte>& binCode);



	FRIShaderPipeline* CreateShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor);
	FRIShaderPipeline* CreateShaderPipeline(const ShaderLibraryModule& shaderModule);

	FRITexture2D* CreateTexture2D(uint32 width, uint32 height, uint32 sampleCount, EFRITextureFormat format, FRIColorDataFormat colorData, FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0), bool cpuWrite = false);
	FRITexture3D* CreateTexture3D(uint32 width, uint32 height, uint32 depth, EFRITextureFormat format, FRIColorDataFormat colorData = FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::Byte), FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0), bool cpuWrite = false, bool render = true);
	FRITexture2DArray* CreateTexture2DArray(uint32 width, uint32 height, uint32 numLayers, EFRITextureFormat format, FRIColorDataFormat colorData, FRICreationDescriptor resourceDescriptor[] = NULL);
	FRITextureCubeMap* CreateTextureCubeMap(uint32 width, uint32 height, uint32 sampleCount, EFRITextureFormat format, FRIColorDataFormat colorData = FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::Byte), FRICreationDescriptor resourceDescriptor[] = NULL);
	
	FRIUniformBuffer* CreateUniformBuffer(FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0));
	FRIFrameBuffer* CreateFrameBuffer(FArray<FRIFrameBufferAttachment> textureAttachments, bool enableDepthRenderBuffer = true);
	FRIFrameBuffer* CreateFrameBuffer(FRIFrameBufferArrayAttachment textureAttachments, bool enableDepthRenderBuffer = true, uint32 MipLevel = 0);
	FRIComputeBuffer* CreateComputeBuffer(EFRIAccess access, size_t StructureStride, FRICreationDescriptor creationDescriptor);


	FRIRasterizerState* CreateRasterizerState(EFRICullMode cullMode, EFRIFillMode fillmode);
	FRIBlendState* CreateBlendState(EFRIAlphaBlend srcBlend, EFRIAlphaBlend dstBlend);
	FRIDepthStencilState* CreateDepthStencilState(EFRIBool depthEnable, EFRIBool stencilEnable);


	FRIVertexDeclaration* CreateVertexDeclaration(FArray<FRIInputDesc> components, FRIVertexShader* shaderSignature);
	void AttachVertexDeclaration(FRIVertexBuffer* geometry, FRIVertexDeclaration* declaration);

	void UniformBufferSubdata(FRIUniformBuffer* buffer, FRIUpdateDescriptor resource);
	void InstanceBufferSubdata(FRIInstanceBuffer* buffer, FRIUpdateDescriptor resource);
	void Texture2DSubdata(FRITexture2D* texture, FRIUpdateDescriptor resource);
	void Texture3DSubdata(FRITexture3D* texture, FRIUpdateDescriptor resource);
	void VertexBufferSubdata(FRIVertexBuffer* buffer, FRIUpdateDescriptor resource);


	void SetRasterizerState(FRIRasterizerState* rasterizer);
	void SetBlendState(FRIBlendState* blend);
	void SetDepthStencilState(FRIDepthStencilState* depth);

	void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);
	void BindFrameBuffer(FRIFrameBuffer* frameBuffer);
	void UnbindFrameBuffer();

	void SetGeometrySource(FRIVertexBuffer* vertexBuffer);
	void SetInstancedGeometrySource(FRIVertexBuffer* vertexBuffer, FRIInstanceBuffer* instanceBuffer);

	void DrawPrimitives(EFRIPrimitiveType primitveType, uint32 vertexCount);
	void DrawPrimitivesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, EFRIIndexType indexType, FRIIndexBuffer* indexBuffer);


	void DrawInstances(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount);
	void DrawInstancesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount, EFRIIndexType indexType, FRIIndexBuffer* indexBuffer);

	void SetShaderPipeline(FRIShaderPipeline* shader);
	void SetShaderUniformBuffer(uint32 slot, FRIUniformBuffer* uniformBuffer, uint32 attachFlags = 63);
	void SetShaderSampler(FRISampler sampler);
	void SetTextureParameterBuffer(FRITexture2D* texture, FRITextureParameterBuffer param);
	void SetTextureParameterBuffer(FRITexture2DArray* texture, FRITextureParameterBuffer param);

	void SetFramebufferTextureLayer(FRIFrameBuffer* tex, uint32 layer);

	void StageResources(FRIUniformBuffer* ubo, FRIMemoryStageDelegate stageLambda);
	void SetUAV(uint32 slot, FRITexture3D* tex);
	void SetUAV(uint32 slot, FRIComputeBuffer* buffer);
	void ClearUAV(uint32 slot);

	void BeginScene();
	void BeginFrame();
	void EndScene();
	void EndFrame();

	void ClearBuffer(FRIFrameBuffer* buffer, Color color);
	void ClearBufferAndSetUAVs(FRIFrameBuffer* buffer, Color color, FRITexture3D** uavs, uint32 slots);

	void FlushMipMaps(FRITexture2D* tex);
	void FlushMipMaps(FRITexture2DArray* tex);
	void FlushMipMaps(FRITexture3D* tex);
	void FlushMipMaps(FRITextureCubeMap* tex);

	void CopyTexture(FRITexture2D* source, FRITexture2D* dest);
	void CopyTexture(FRITexture2DArray* source, FRITexture2DArray* dest);
	void CopyTexture(FRITexture3D* source, FRITexture3D* dest);
	void CopyBuffer(FRIBuffer* source, FRIBuffer* dest);

	void DispatchCompute(uint32 xThreads, uint32 yThreads, uint32 zThreads);

};