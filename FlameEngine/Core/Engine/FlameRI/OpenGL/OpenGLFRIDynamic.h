#pragma once


#include "../FRIDynamicAllocator.h"
#include "OpenGLFRIResource.h"

struct OpenGLFRIDynamicAllocator : FRIDynamicAllocator
{
public:
	FRIIndexBuffer*		CreateIndexBuffer(uint32 Size, uint32 Usage, FRICreationDescriptor resourceDescriptor);
	FRIVertexBuffer*		CreateVertexBuffer(uint32 Size, uint32 Usage, FRICreationDescriptor resourceDescriptor);
	FRIInstanceBuffer* CreateInstanceBuffer(uint32 Size, uint32 LayoutStride, FRICreationDescriptor resourceDescriptor);


	FRIVertexShader*		CreateVertexShader(const FArray<byte>& binCode);
	FRIPixelShader*		CreatePixelShader(const FArray<byte>& binCode);
	FRIGeometryShader*	CreateGeometryShader(const FArray<byte>& binCode);
	FRIHullShader*		CreateHullShader(const FArray<byte>& binCode);
	FRIDomainShader*		CreateDomainShader(const FArray<byte>& binCode);
	FRIComputeShader*		CreateComputeShader(const FArray<byte>& binCode);



	FRIShaderPipeline*	CreateShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor);
	FRIShaderPipeline*	CreateShaderPipeline(const ShaderLibraryModule& shaderModule);

	FRITexture2D*			CreateTexture2D(uint32 width, uint32 height, uint32 sampleCount, EFRITextureFormat format, FRIColorDataFormat colorData = FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::Byte), FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0), bool cpuWrite = false);
	FRITexture3D*			CreateTexture3D(uint32 width, uint32 height, uint32 depth, FRICreationDescriptor resourceDescriptor);
	FRITexture2DArray*	CreateTexture2DArray(uint32 width, uint32 height, uint32 numLayers, EFRITextureFormat format, FRIColorDataFormat colorData = FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::Byte), FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0));
	FRIUniformBuffer*		CreateUniformBuffer(FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0));
	FRIFrameBuffer*		CreateFrameBuffer(FArray<FRIFrameBufferAttachment> textureAttachments, bool enableDepthRenderBuffer = true);
	FRIFrameBuffer*		CreateFrameBuffer(FRIFrameBufferArrayAttachment textureAttachments, bool enableDepthRenderBuffer = true);


	FRIRasterizerState* CreateRasterizerState(EFRICullMode cullMode, EFRIFillMode fillmode);
	FRIBlendState* CreateBlendState(EFRIAlphaBlend srcBlend, EFRIAlphaBlend dstBlend);
	FRIDepthStencilState* CreateDepthStencilState(EFRIBool depth, EFRIBool stencil);



	FRIVertexDeclaration* CreateVertexDeclaration(FArray<FRIVertexDeclarationDesc> components, FRIVertexShader* shaderSignature);
	void AttachVertexDeclaration(FRIVertexBuffer* geometry, FRIVertexDeclaration* declaration);


	void UniformBufferSubdata(FRIUniformBuffer* buffer, FRIUpdateDescriptor resource);
	void InstanceBufferSubdata(FRIInstanceBuffer* buffer, FRIUpdateDescriptor resource);
	void Texture2DSubdata(FRITexture2D* texture, FRIUpdateDescriptor resource);
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
	void SetShaderUniformBuffer(uint32 slot, FRIUniformBuffer* uniformBuffer);
	void SetShaderSampler(FUniformSampler* sampler);
	void SetTextureParameterBuffer(FRITexture2D* texture, FRITextureParameterBuffer param);
	void SetTextureParameterBuffer(FRITexture2DArray* texture, FRITextureParameterBuffer param);

	void SetFramebufferTextureLayer(FRIFrameBuffer* tex, uint32 layer);

	void StageResources(FRIUniformBuffer* ubo, FRIResourceStageLambda stageLambda);


	uint32 GetShaderUniformParameterLocation(FRIShaderPipeline* shader, FAnsiString uniformName);



	void BeginScene();
	void BeginFrame();
	void EndScene();
	void EndFrame();

	void ClearBuffer(FRIFrameBuffer* buffer, Color color);

	void SetBackCull(bool back);

	void FlushMipMaps(FRITexture2D* tex);
	void FlushMipMaps(FRITexture2DArray* tex);

	void CopyTexture(FRITexture2D* source, FRITexture2D* dest);

};