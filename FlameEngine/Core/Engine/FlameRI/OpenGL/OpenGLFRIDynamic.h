#pragma once


#include "../FRIDynamicAllocator.h"
#include "OpenGLFRIResource.h"

struct OpenGLFRIDynamicAllocator : FRIDynamicAllocator
{
public:
	FResourceIndexBuffer*		DynamicCreateIndexBuffer(uint32 Size, uint32 Usage, FResourceCreationDescriptor resourceDescriptor);
	//FResourceIndexBuffer*		DynamicCreateIndexBuffer(uint32 Size, uint32 Usage, uint32* DataArray);
	FResourceVertexBuffer*		DynamicCreateVertexBuffer(uint32 Size, uint32 Usage, FResourceCreationDescriptor resourceDescriptor);


	/*FResourceVertexShader*		DynamicCreateVertexShader(const FArray<byte>& binCode);
	FResourcePixelShader*		DynamicCreatePixelShader(const FArray<byte>& binCode);
	FResourceGeometryShader*	DynamicCreateGeometryShader(const FArray<byte>& binCode);
	FResourceHullShader*		DynamicCreateHullShader(const FArray<byte>& binCode);
	FResourceDomainShader*		DynamicCreateDomainShader(const FArray<byte>& binCode);
	FResourceComputeShader*		DynamicCreateComputeShader(const FArray<byte>& binCode);*/


	FResourceVertexShader*		DynamicCreateVertexShader	(const FAnsiString& binCode);
	FResourcePixelShader*		DynamicCreatePixelShader	(const FAnsiString& binCode);
	FResourceGeometryShader*	DynamicCreateGeometryShader	(const FAnsiString& binCode);
	FResourceHullShader*		DynamicCreateHullShader		(const FAnsiString& binCode);
	FResourceDomainShader*		DynamicCreateDomainShader	(const FAnsiString& binCode);
	FResourceComputeShader*		DynamicCreateComputeShader	(const FAnsiString& binCode);



	FResourceShaderPipeline*	DynamicCreateShaderPipeline(FResourceShaderPipelineCreationDescriptor descriptor);
								
	FResourceTexture2D*			DynamicCreateTexture2D(uint32 width, uint32 height, uint32 sampleCount, FResourceTextureColorDescriptor colorDescriptor, FResourceCreationDescriptor resourceDescriptor = FResourceCreationDescriptor(NULL, 0));
	FResourceTexture3D*			DynamicCreateTexture3D(uint32 width, uint32 height, uint32 depth, FResourceCreationDescriptor resourceDescriptor);

	FResourceTexture2DArray*	DynamicCreateTexture2DArray(uint32 width, uint32 height, uint32 numLayers, FResourceTextureColorDescriptor colorDescriptor, FResourceCreationDescriptor resourceDescriptor = FResourceCreationDescriptor(NULL, 0));


	FResourceFrameBuffer*		DynamicCreateFrameBuffer(FArray<FResourceFrameBufferTextureAttachment> textureAttachments, bool enableDepthRenderBuffer = true);

	void AttachVertexDeclaration(FResourceVertexDeclaration declaration);


	FArray<byte> PrecacheShader(FArray<byte> indata, EFRIResourceShaderType type);

	void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);
	void BindFrameBuffer(FResourceFrameBuffer* frameBuffer);
	void UnbindFrameBuffer();

	void SetGeometrySource(FResourceVertexBuffer* vertexBuffer);
	void DrawPrimitives(uint32 elementType, uint32 elementCount);
	void DrawPrimitivesIndexed(uint32 elementType, uint32 elementCount, uint32 indexType, FResourceIndexBuffer* indexBuffer);

	void SetShaderPipeline(FResourceShaderPipeline* shader);
	void SetShaderUniformBuffer(FResourceUniformBuffer* uniformBuffer);
	void SetShaderUniformParameter(FUniformParameter* paramater);
	void SetShaderSampler(FUniformSampler* sampler);
	void SetTextureParameterBuffer(FResourceTexture2D* texture, FResourceTextureParameterBuffer param);
	void SetTextureParameterBuffer(FResourceTexture2DArray* texture, FResourceTextureParameterBuffer param);

	void SetFramebufferTextureLayer(FResourceTexture2DArray* tex, uint32 layer);

	void AttachLayeredTexture(FResourceTexture2DArray* tex);


	uint32 GetShaderUniformParameterLocation(FResourceShaderPipeline* shader, FAnsiString uniformName);



	void BeginScene();
	void BeginFrame();
	void EndScene();
	void EndFrame();

	void ClearBuffer(FResourceFrameBuffer* buffer, Color color);

	void SetBackCull(bool back);

	void FlushMipMaps(FResourceTexture2D* tex);
	void FlushMipMaps(FResourceTexture2DArray* tex);

};