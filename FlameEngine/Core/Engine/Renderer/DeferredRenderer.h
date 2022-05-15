#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"
#include "Common/Geometry/VertexComponent.h"
#include "../GameSystem/Common/Scene.h"
#include "../GameSystem/Environment/EnvironmentMap.h"

#include "Common/RenderOutputLayer.h"
#include "AtmosphereRenderer.h"
#include "Core/Engine/ContentSystem/ImportScripts/ShaderLibrary.h"
#include "Deferred/SMAA/SMAA.h"
#include "Services/HBAOPlus/HBAOPlusInterface.h"
#include "../GameSystem/RenderObject.h"
#include "BlurRenderer.h"
#include "../PCI/PCI.h"



EXPORT(class, DeferredRenderer) : public RenderModule, public IProperties
{

	struct _DRUBuffers
	{
		FRIStageBuffer DLight;
		FRIStageBuffer PLight;
		FRIStageBuffer SLight;
		FRIStageBuffer CascadeData;
		FRIStageBuffer LightingConstants;
		FRIStageBuffer VXGI;
		FRIStageBuffer Transform;
		FRIStageBuffer JointData;
		FRIStageBuffer CameraMatrix;
		FRIStageBuffer Material;

		void Create(DeferredRenderer* renderer, FRIContext* renderContext);

	} UBuffers;

	struct _DRShaders
	{
		FRIShaderPipelineRef Lighting;
		FRIShaderPipelineRef GShader;
		FRIShaderPipelineRef GSkinnedShader;
		FRIShaderPipelineRef SMShader;
		FRIShaderPipelineRef SMSkinnedShader;
		FRIShaderPipelineRef PostProcess;

		void Create(DeferredRenderer* renderer, FRIContext* renderContext);

	} Shaders;


	struct _DRFBO
	{
		FRIFrameBufferRef PostProcess;                                                                                                                                                                                               
		FRIFrameBufferRef Lighting;
		FRIFrameBufferRef GBuffer;
		FRIFrameBufferRef SMBuffer;
		FRIFrameBufferRef AOBuffer;
		FRIFrameBufferRef TransluscentBuffer;

		FRITexture2DRef AOTexture;
		FRITexture2DRef PostProcessTex;
		FRITexture2DRef LightingTex;
		FRITexture2DRef Albedo;
		FRITexture2DRef Normal;
		FRITexture2DRef MetallicRoughness;
		FRITexture2DRef Emissive;
		FRITexture2DRef LightAndTransluscentTex;
		FRITexture2DArrayRef ShadowmapArray;

		FRITextureCubeMapRef EnvCubemap;

		FRITexture2DRef BRDFLut;

		void Create(DeferredRenderer* renderer, FRIContext* renderContext);

	} FrameBuffers;


	FHashMap<FString8, RenderOutputLayer> outputLayers;


	PropertyInt(ShadowmapResolution, 2048);
	PropertyBool(AOEnabled, true);

public:

	~DeferredRenderer();

	void CreateResources(FRIContext* renderContext);
	void RecreateResources(FRIContext* renderContext, FRIContext* prevContext = 0);

	void CreateRenderUtil(FRIContext* renderContext);

	void BeginRender(FRICommandList & cmdList);
	void Render(FRICommandList& cmdList);
	void EndRender(FRICommandList & cmdList);


	void RenderGBuffer(FRICommandList& cmdList, const Camera& camera);
	void RenderShadowmaps(FRICommandList& cmdList, const DirectionalLight& Sun);
	void RenderTransluscency(FRICommandList& cmdList);
	void RenderPostProcess(FRICommandList& cmdList);
	void RenderLighting(FRICommandList& cmdList, const Camera& Camera, const DirectionalLight& Sun, const EnvironmentMap& env);
	void RenderGI(FRICommandList& cmdList, const Camera& Camera);
	void Present(FRICommandList& cmdList);


	void RenderGeometry(FRICommandList& cmdList, GRenderMode renderMode);
	void RenderGeometrySkinned(FRICommandList& cmdList);
	void RenderEnvironmentStatic(FRICommandList& cmdList, GRenderMode renderMode);

	void UnbindSamplers(FRICommandList& cmdList, int32 startSlot, int32 endSlot);
	void StageLightData(FRICommandList& cmdList, const Camera& camera, const DirectionalLight Sun);

	RenderOutputLayer GetOutputLayer(const FString8& layer);

	FRIBlendStateRef DefaultBlend;
	FRIRasterizerStateRef DefaultRasterizer;
	FRIRasterizerStateRef SMRasterizer;

	FRIDepthStencilStateRef DisableDepth;
	FRIDepthStencilStateRef DefaultDepth;


	FViewportRect Viewport;
	FViewportRect ShadowmapViewport;

	BlurRenderer blurRenderer;
	AtmosphereRenderer atmosphereRenderer;
	ParticleRenderer particleRenderer;
	FUniquePtr<HBAOPlus> hbaoRenderer;
	SMAA smaa;


	FTimeSpan stageTime, giTime, smTime, gTime, lightTime, ppTime, presentTime, smLayerTime[5], smaaTime, totalTime, testTime;
};