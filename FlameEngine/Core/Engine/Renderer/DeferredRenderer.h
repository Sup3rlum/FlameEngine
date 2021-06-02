#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"
#include "Common/Geometry/VertexComponent.h"
#include "../GameSystem/Common/Scene.h"
#include "RenderSystems.h"
#include "Common/RenderUtil.h"

#include "Services/HBAOPlus/HBAOPlusInterface.h"
#include "Services/BoundingVolumeDebugService.h"

#include "AtmosphereRenderer.h"

#include "Core/Engine/ContentSystem/Client/AssetImportScripts/ShaderLibrary.h"

typedef FRITexture2D* FRITexture2DRef;
typedef FRITexture2DArray* FRITexture2DArrayRef;

struct FSceneRenderBuffer
{
	FRITexture2DRef DepthBuffer;
	FRITexture2DRef NormalBuffer;
	FRITexture2DRef AlbedoBuffer;
	FRITexture2DRef SpecularBuffer;
	FRITexture2DRef SSAOBuffer;

	FRITexture2DArrayRef ShadowmapCascadeArray;
};


struct FRenderPipeline
{
	FRIShaderPipeline* Handle;
	FRIFrameBuffer* FrameBuffer;
};


struct FSkinnedShadowmapGenPipeline : public FRenderPipeline {};
struct FShadowmapGenPipeline : public FRenderPipeline
{
	FRIUniformBuffer* CascadeCameraBuffer;
	FViewportRect Viewport;
};
struct FCombineRenderPipeline : public FRenderPipeline
{
	FRIUniformBuffer* ConstantBuffer;
};
struct FHBAORenderPipeline : public FRenderPipeline
{
	HBAOPlus* HbaoService;
};






EXPORT(class, DeferredRenderer) : public RenderModule
{
public:

	void CreateResources(FRIContext* renderContext);

	void CreateCombineStage();
	void CreateHBAOStage();

	void CreateShadowmapStage();
	void CreateGBufferGenStage();
	void CreateCombineGenStage();

	void BeginRender(FRICommandList & cmdList);
	void Render(FRICommandList& cmdList);
	void EndRender(FRICommandList & cmdList);

	void GBufferGen(FRICommandList& cmdList);
	void RenderAO(FRICommandList& cmdList);

	void RenderCombine(FRICommandList& cmdList);
	void DrawScreenQuad(FRICommandList& cmdList );

	void AttachToScene(Scene* scene);


	FRenderPipeline			GeomGenPipeline;
	FRenderPipeline			SkinnedGeomGenPipeline;

	FHBAORenderPipeline				HBAOGenPipeline;
	FCombineRenderPipeline			CombinePipeline;
	FShadowmapGenPipeline			ShadowmapPipeline;
	FSkinnedShadowmapGenPipeline	SkinnedShadowmapPipeline;

	FSceneRenderBuffer BufferTextures;


	FRIUniformBuffer* cameraMatrixBuffer;
	FRIUniformBuffer* transformBuffer;
	FRIUniformBuffer* jointBuffer;


	AtmosphereRenderer atmosphereRenderer;


	FRIContext* renderContext;
	FRIBlendState* blendState;

	FRenderSystemGeom* Geometry;
	FRenderSystemSkinnedGeom* SkinnedGeometry;
	FRenderSystemSM* SMGeometry;
	FRenderSystemSkinnedSM* SkinnedSMGeometry;

	Scene* scene = NULL;
};