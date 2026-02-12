#pragma once
#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"




#include "../Environment/AtmosphereRenderer.h"
#include "Core/Engine/ContentSystem/ImportScripts/ShaderLibrary.h"
#include "../SMAA/SMAA.h"
#include "../HBAOPlus/HBAOPlusInterface.h"
#include "../Effects/BlurRenderer.h"
#include "../../PCI/PCI.h"



class FLAME_API DeferredRenderer : public IProperties
{
	struct DeferredPipelines
	{
		FRIPipelineStateObject* GShader;
		FRIPipelineStateObject* GSkinnedShader;
		FRIPipelineStateObject* SMShader;
		FRIPipelineStateObject* SMSkinnedShader;
		FRIPipelineStateObject* Lighting;
		FRIPipelineStateObject* PostProcess;
	} Pipelines;

	struct DeferredCBuffers
	{
		FRIConstantBuffer* CameraMatrix;
		FRIConstantBuffer* Transform;
		FRIConstantBuffer* JointData;
		FRIConstantBuffer* DLight;
		FRIConstantBuffer* PLight;
		FRIConstantBuffer* SLight;
		FRIConstantBuffer* LightingConstants;
	} CBuffers;

	struct RenderSurface2D
	{
		FRITexture2D* Surface;
		FRIShaderResourceView* SRV;
		FRIRenderTargetView* RTV;
	};

	struct DepthSurface2D
	{
		FRITexture2D* Surface;
		FRIShaderResourceView* SRV;
		FRIRenderTargetView* RTV;
		FRIDepthStencilView* DSV;
	};

	DepthSurface2D Depth;
	RenderSurface2D Normal;
	RenderSurface2D Albedo;
	RenderSurface2D MetallicRoughness;
	RenderSurface2D Emissive;
	RenderSurface2D Lighting;
	RenderSurface2D BRDF;


	PropertyInt(ShadowmapResolution, 2048);
	PropertyBool(AOEnabled, true);

	~DeferredRenderer();

	void CreateResources(FRIContext* renderContext);
	void CreateRenderUtil(FRIContext* renderContext);

	void RenderGBuffer(FRICommandList& cmdList, const Camera& camera);
	void RenderGeometry(FRICommandList& cmdList, const Camera& camera);
	void RenderPostProcess(FRICommandList& cmdList);
	void RenderLighting(FRICommandList& cmdList, const Camera& Camera);

	void StageLightData(FRICommandList& cmdList, const Camera& camera, const DirectionalLight Sun);

	void BeginRender(FRICommandList& cmdList);
	void Render(FRICommandList& cmdList);
	void EndRender(FRICommandList& cmdList);
	void AttachToScene(Scene* scene);

	FViewportRect Viewport;
	FViewportRect ShadowmapViewport;
	Scene* pScene;

};