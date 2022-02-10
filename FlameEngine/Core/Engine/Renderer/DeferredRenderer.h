#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"
#include "Common/Geometry/VertexComponent.h"
#include "../GameSystem/Common/Scene.h"



#include "DebugRenderer.h"
#include "AtmosphereRenderer.h"
#include "Core/Engine/ContentSystem/Client/AssetImportScripts/ShaderLibrary.h"
#include "Deferred/SMAA/SMAA.h"
#include "VXGI/VXGIRenderer.h"


struct VXGI
{
	bool Enable;

	float MaxTracingDistance;
	float BounceStrength;
	float AOFalloff;
	float AOAlpha;
	float SamplingFactor;
	float ShadowTolerance;
	float ShadowAperture;

};

EXPORT(class, DeferredRenderer) : public RenderModule
{

	struct _DRUBuffers
	{
		FRIUniformBuffer* DLight;
		FRIUniformBuffer* PLight;
		FRIUniformBuffer* SLight;
		FRIUniformBuffer* CascadeData;
		FRIUniformBuffer* LightingConstants;
		FRIUniformBuffer* VXGI;
		FRIUniformBuffer* Transform;
		FRIUniformBuffer* JointData;
		FRIUniformBuffer* CameraMatrix;
		FRIUniformBuffer* Material;

		void Create(FRIContext* renderContext);

	} UBuffers;

	struct _DRShaders
	{
		FRIShaderPipeline* Lighting;
		FRIShaderPipeline* GShader;
		FRIShaderPipeline* GSkinnedShader;
		FRIShaderPipeline* SMShader;
		FRIShaderPipeline* SMSkinnedShader;
		FRIShaderPipeline* PostProcess;

		void Create(FRIContext* renderContext);

	} Shaders;


	struct _DRFBO
	{
		FRIFrameBuffer* PostProcess;                                                                                                                                                                                               
		FRIFrameBuffer* Lighting;
		FRIFrameBuffer* GBuffer;
		FRIFrameBuffer* SMBuffer;
		FRIFrameBuffer* AOBuffer;

		FRITexture2D* AOTexture;
		FRITexture2D* PostProcessTex;
		FRITexture2D* LightingTex;
		FRITexture2D* Albedo;
		FRITexture2D* Normal;
		FRITexture2D* MetallicRoughness;
		FRITexture2D* Emissive;
		FRITexture2DArray* ShadowmapArray;

		FRITexture2D* BRDFLut;

		void Create(FRIContext* renderContext);

	} FrameBuffers;


public:

	

	void CreateResources(FRIContext* renderContext);
	void RecreateResources(FRIContext* renderContext, FRIContext* prevContext = 0);

	void CreateRenderUtil(FRIContext* renderContext);

	void BeginRender(FRICommandList & cmdList);
	void Render(FRICommandList& cmdList);
	void EndRender(FRICommandList & cmdList);


	void RenderGBuffer(FRICommandList& cmdList);
	void RenderShadowmaps(FRICommandList& cmdList);
	void RenderTransluscency(FRICommandList& cmdList);
	void RenderPostProcess(FRICommandList& cmdList);
	void RenderLighting(FRICommandList& cmdList);
	void RenderGI(FRICommandList& cmdList);
	void Present(FRICommandList& cmdList);


	void RenderGeometry(FRICommandList& cmdList);
	void RenderGeometrySkinned(FRICommandList& cmdList);
	void RenderEnvironmentStatic(FRICommandList& cmdList);

	void UnbindSamplers(FRICommandList& cmdList, int32 startSlot, int32 endSlot);
	void StageLightData(FRICommandList& cmdList);


	FRIBlendState* DefaultBlend;
	FRIRasterizerState* DefaultRasterizer;
	FRIDepthStencilState* DisableDepth;
	FRIDepthStencilState* DefaultDepth;


	FRenderSystemGeom* Geometry;
	FRenderSystemSkinnedGeom* SkinnedGeometry;



	VXGI GlobalIllumination;


	FViewportRect Viewport;
	FViewportRect ShadowmapViewport;

	AtmosphereRenderer atmosphereRenderer;
	VXGIRenderer vxgiRenderer;
	DebugRenderer debugRenderer;
	SMAA smaa;
};