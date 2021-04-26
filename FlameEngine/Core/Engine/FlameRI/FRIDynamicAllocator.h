#pragma once


#include "Core/Common/CoreCommon.h"
#include "FRIResource.h"





class FRIDynamicAllocator
{
public:
	virtual FResourceIndexBuffer* DynamicCreateIndexBuffer(uint32 IndexCount, uint32 Usage, FResourceCreationDescriptor resourceDescriptor) = 0;
	//virtual FResourceIndexBuffer* DynamicCreateIndexBuffer(uint32 IndexCount, uint32 Usage, uint32* DataArray) = 0;
	virtual FResourceVertexBuffer* DynamicCreateVertexBuffer(uint32 Size, uint32 Usage, FResourceCreationDescriptor resourceDescriptor) = 0;


	/*virtual FResourceVertexShader* DynamicCreateVertexShader(const FArray<byte>& binCode) = 0;
	virtual FResourcePixelShader* DynamicCreatePixelShader(const FArray<byte>& binCode) = 0;
	virtual FResourceGeometryShader* DynamicCreateGeometryShader(const FArray<byte>& binCode) = 0;
	virtual FResourceHullShader* DynamicCreateHullShader(const FArray<byte>& binCode) = 0;
	virtual FResourceDomainShader* DynamicCreateDomainShader(const FArray<byte>& binCode) = 0;
	virtual FResourceComputeShader* DynamicCreateComputeShader(const FArray<byte>& binCode) = 0;*/


	virtual FResourceVertexShader* DynamicCreateVertexShader(const FAnsiString& binCode) = 0;
	virtual FResourcePixelShader* DynamicCreatePixelShader(const FAnsiString& binCode) = 0;
	virtual FResourceGeometryShader* DynamicCreateGeometryShader(const FAnsiString& binCode) = 0;
	virtual FResourceHullShader* DynamicCreateHullShader(const FAnsiString& binCode) = 0;
	virtual FResourceDomainShader* DynamicCreateDomainShader(const FAnsiString& binCode) = 0;
	virtual FResourceComputeShader* DynamicCreateComputeShader(const FAnsiString& binCode) = 0;


	virtual FResourceShaderPipeline* DynamicCreateShaderPipeline(FResourceShaderPipelineCreationDescriptor descriptor) = 0;

	virtual FResourceTexture2D* DynamicCreateTexture2D(uint32 width, uint32 height, uint32 sampleCount, FResourceTextureColorDescriptor colorDescriptor, FResourceCreationDescriptor resourceDescriptor = FResourceCreationDescriptor(NULL, 0)) = 0;
	virtual FResourceTexture3D* DynamicCreateTexture3D(uint32 width, uint32 height, uint32 depth, FResourceCreationDescriptor resourceDescriptor) = 0;
	
	virtual FResourceTexture2DArray* DynamicCreateTexture2DArray(uint32 width, uint32 height, uint32 numLayers, FResourceTextureColorDescriptor colorDescriptor, FResourceCreationDescriptor resourceDescriptor = FResourceCreationDescriptor(NULL, 0)) = 0;
 
	virtual FResourceFrameBuffer* DynamicCreateFrameBuffer(FArray<FResourceFrameBufferTextureAttachment> textureAttachments, bool enableDepthRenderBuffer = true) = 0;

	virtual void AttachVertexDeclaration(FResourceVertexDeclaration declaration) = 0;


	virtual FArray<byte> PrecacheShader(FArray<byte> indata, EFRIResourceShaderType type) = 0;


	virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
	virtual void BindFrameBuffer(FResourceFrameBuffer* frameBuffer) = 0;
	virtual void UnbindFrameBuffer() = 0;

	virtual void SetGeometrySource(FResourceVertexBuffer* vertexBuffer) = 0;
	virtual void DrawPrimitives(uint32 elementType, uint32 elementCount) = 0;
	virtual void DrawPrimitivesIndexed(uint32 elementType, uint32 elementCount, uint32 indexType, FResourceIndexBuffer* indexBuffer) = 0;

	virtual void SetShaderPipeline(FResourceShaderPipeline* shader) = 0;
	virtual void SetShaderUniformBuffer(FResourceUniformBuffer* uniformBuffer) = 0;
	virtual void SetShaderUniformParameter(FUniformParameter* paramater) = 0;
	virtual void SetShaderSampler(FUniformSampler* sampler) = 0;
	virtual void SetTextureParameterBuffer(FResourceTexture2D* texture, FResourceTextureParameterBuffer param) = 0;
	virtual void SetTextureParameterBuffer(FResourceTexture2DArray* texture, FResourceTextureParameterBuffer param) = 0;

	virtual void SetFramebufferTextureLayer(FResourceTexture2DArray* tex, uint32 layer) = 0;


	virtual void AttachLayeredTexture(FResourceTexture2DArray* tex) = 0;

	virtual uint32 GetShaderUniformParameterLocation(FResourceShaderPipeline* shader, FAnsiString uniformName) = 0;


	virtual void BeginScene() = 0;
	virtual void BeginFrame() = 0;
	virtual void EndScene() = 0;
	virtual void EndFrame() = 0;

	virtual void ClearBuffer(FResourceFrameBuffer* buffer, Color color) = 0;


	virtual void SetBackCull(bool back) = 0;

	virtual void FlushMipMaps(FResourceTexture2D* tex) = 0;
	virtual void FlushMipMaps(FResourceTexture2DArray* tex) = 0;

};