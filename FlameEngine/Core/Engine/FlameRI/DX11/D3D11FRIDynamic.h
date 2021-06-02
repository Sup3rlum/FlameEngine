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


	FRIVertexShader* CreateVertexShader(const FArray<byte>& binCode);
	FRIPixelShader* CreatePixelShader(const FArray<byte>& binCode);
	FRIGeometryShader* CreateGeometryShader(const FArray<byte>& binCode);
	FRIHullShader* CreateHullShader(const FArray<byte>& binCode);
	FRIDomainShader* CreateDomainShader(const FArray<byte>& binCode);
	FRIComputeShader* CreateComputeShader(const FArray<byte>& binCode);



	FRIShaderPipeline* CreateShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor);
	FRIShaderPipeline* CreateShaderPipeline(const ShaderLibraryModule& shaderModule);

	FRITexture2D* CreateTexture2D(uint32 width, uint32 height, uint32 sampleCount, EFRITextureFormat format, FRIColorDataFormat colorData, FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0));
	FRITexture3D* CreateTexture3D(uint32 width, uint32 height, uint32 depth, FRICreationDescriptor resourceDescriptor);
	FRITexture2DArray* CreateTexture2DArray(uint32 width, uint32 height, uint32 numLayers, EFRITextureFormat format, FRIColorDataFormat colorData, FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0));
	FRIUniformBuffer* CreateUniformBuffer(FRICreationDescriptor resourceDescriptor = FRICreationDescriptor(NULL, 0));
	FRIFrameBuffer* CreateFrameBuffer(FArray<FRIFrameBufferAttachment> textureAttachments, bool enableDepthRenderBuffer = true);
	FRIFrameBuffer* CreateFrameBuffer(FRIFrameBufferArrayAttachment textureAttachments, bool enableDepthRenderBuffer = true);

	FRIRasterizerState* CreateRasterizerState(EFRICullMode cullMode, EFRIFillMode fillmode);
	FRIBlendState* CreateBlendState(EFRIAlphaBlend srcBlend, EFRIAlphaBlend dstBlend);



	FRIVertexDeclaration* CreateVertexDeclaration(FArray<FRIVertexDeclarationComponent> components, FRIVertexShader* shaderSignature);
	void AttachVertexDeclaration(FRIVertexBuffer* geometry, FRIVertexDeclaration* declaration);

	void UniformBufferSubdata(FRIUniformBuffer* buffer, FRIUpdateDescriptor resource);



	void SetRasterizerState(FRIRasterizerState* rasterizer);
	void SetBlendState(FRIBlendState* blend);

	void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);
	void BindFrameBuffer(FRIFrameBuffer* frameBuffer);
	void UnbindFrameBuffer();

	void SetGeometrySource(FRIVertexBuffer* vertexBuffer);
	void DrawPrimitives(uint32 elementType, uint32 elementCount);
	void DrawPrimitivesIndexed(uint32 elementType, uint32 elementCount, uint32 indexType, FRIIndexBuffer* indexBuffer);

	void SetShaderPipeline(FRIShaderPipeline* shader);
	void SetShaderUniformBuffer(uint32 slot, FRIUniformBuffer* uniformBuffer);
	void SetShaderSampler(FUniformSampler* sampler);
	void SetTextureParameterBuffer(FRITexture2D* texture, FRITextureParameterBuffer param);
	void SetTextureParameterBuffer(FRITexture2DArray* texture, FRITextureParameterBuffer param);

	void SetFramebufferTextureLayer(FRIFrameBuffer* tex, uint32 layer);

	void StageResources(FRIResourceStageLambda stageLambda);


	void BeginScene();
	void BeginFrame();
	void EndScene();
	void EndFrame();

	void ClearBuffer(FRIFrameBuffer* buffer, Color color);

	void SetBackCull(bool back);

	void FlushMipMaps(FRITexture2D* tex);
	void FlushMipMaps(FRITexture2DArray* tex);




};