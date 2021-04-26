#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Common/Viewport.h"
#include "Common/Geometry/VertexComponent.h"
#include "../GameSystem/Common/Scene.h"
#include "../GameSystem/MeshComponent.h"
#include "../GameSystem/SkinnedMeshComponent.h"
#include "RenderComponent.h"
#include "Common/RenderUtil.h"

#include "Services/HBAOPlus/HBAOPlusInterface.h"
#include "Services/BoundingVolumeDebugService.h"

#include "AtmosphereRenderer.h"
#include "SATGenerator.h"

typedef FResourceTexture2D* FResourceTexture2DRef;
typedef FResourceTexture2DArray* FResourceTexture2DArrayRef;

struct FSceneRenderBuffer
{
	FResourceTexture2DRef DepthBuffer;
	FResourceTexture2DRef NormalBuffer;
	FResourceTexture2DRef AlbedoBuffer;
	FResourceTexture2DRef SpecularBuffer;
	FResourceTexture2DRef SSAOBuffer;

	FResourceTexture2DArrayRef ShadowmapCascadeArray;
};


struct FBaseRenderPipeline
{
	FResourceShaderPipeline* Handle;
	FResourceFrameBuffer* FrameBuffer;
};


struct FGeometryRenderPipeline : public FBaseRenderPipeline
{
	uint32 ViewLoc;
	uint32 ProjLoc;
	uint32 WorldLoc;
};
struct FSkinnedGeometryRenderPipeline : public FBaseRenderPipeline
{
	uint32 ViewLoc;
	uint32 ProjLoc;
	uint32 WorldLoc;

	uint32 JointBufferLoc;

};




struct FCombineRenderPipeline : public FBaseRenderPipeline
{
	uint32 DirectionalLightsDirection;
	uint32 DirectionalLightsColor;
	uint32 DirectionalLightsIntensity;

	FStaticArray<uint32, 5> DirectionalLightsVPMatrix;
	FStaticArray<uint32, 5> DirectionalLightsDepth;


	FStaticArray<uint32, 5> ColorsLoc;


	uint32 PointPosition;
	uint32 PointColor;
	uint32 PointIntensity;
	uint32 PointRadius;

	uint32 InverseView;
	uint32 InverseProj;

	uint32 CamPos;
	uint32 View;
};


struct FHBAORenderPipeline : public FBaseRenderPipeline
{
	HBAOPlus* HbaoService;
};

struct FShadowmapGenPipeline : public FBaseRenderPipeline
{
	uint32 ViewLoc;
	uint32 ProjLoc;
	uint32 WorldLoc;
};



EXPORT(class, DeferredRenderer)
{
public:
	void CreateResources(FRIContext* renderContext);

	void BeginRender(FRICommandList& cmdList, Scene* sceneToRender);
	void EndRender(FRICommandList & cmdList);

	void GBufferGen(FRICommandList& cmdList);
	void RenderAO(FRICommandList& cmdList);


	void RenderCombine(FRICommandList& cmdList);


	void DrawScreenQuad(FRICommandList& cmdList );
	void DrawDebugQuad(FRICommandList & cmdList, FVector2 pos, FVector2 size, FResourceTexture2D * tex);
	void DrawDebugQuadLayer(FRICommandList& cmdList, FVector2 pos, FVector2 size, FResourceTexture2DArray* tex, uint32 layer);


	FGeometryRenderPipeline GeomGenPipeline;
	FSkinnedGeometryRenderPipeline SkinnedGeomGenPipeline;

	FHBAORenderPipeline	HBAOGenPipeline;
	FCombineRenderPipeline CombinePipeline;
	FShadowmapGenPipeline ShadowmapPipeline;

	FSceneRenderBuffer BufferTextures;


	FResourceShaderPipeline* quadDrawPipeline;
	FResourceShaderPipeline* quadDrawArrayPipeline;
	uint32 TransformLoc;
	uint32 ProjLoc;

	uint32 TransformLocArray;
	uint32 ProjLocArray;
	uint32 LayerLocArray;
	uint32 MaxLayerLocArray;

	BoundingVolumeDebugService* serv;


	FArray<FMatrix4> matrices;

	AtmosphereRenderer atmosphereRenderer;
	SATGenerator satGenerator;
};