#pragma once

#include "Definitions.h"

#include "../Common/RenderUtil.h"
#include "../RenderSystems.h"


#define UBO_SLOT_CAMERA 0
#define UBO_SLOT_TRANSFORM 1
#define UBO_SLOT_CASCADE_CAMERA 2
#define UBO_SLOT_JOINTS 3
#define UBO_SLOT_COMBINE 4
#define UBO_SLOT_EMISSION 5
#define UBO_SLOT_POINT_LIGHT 6
#define UBO_SLOT_DIR_LIGHT 7

#define RS_SLOT_DEPTH 0
#define RS_SLOT_NORMAL 1
#define RS_SLOT_ALBEDO 2
#define RS_SLOT_METALLIC_ROUGHNESS 3
#define RS_SLOT_EMISSIVE 4
#define RS_SLOT_SHADOWMAP 5
#define RS_SLOT_AO 6
#define RS_SLOT_LIT_SCENE 7
#define RS_SLOT_ENVIRONMENT 8
#define RS_SLOT_AA 9
#define RS_SLOT_TRANSLUSCENCY 10
#define RS_SLOT_FINAL 11



struct FTransformBufferStruct
{
	FMatrix4 World;
	FMatrix4 WorldInverseTranspose;
};




class DRStageGBuffer : public FRenderStage
{

	FViewportRect Viewport;
	FRIShaderPipeline* GShader;
	FRIShaderPipeline* GSkinnedShader;

	FRIFrameBuffer* FrameBuffer;

	FRITexture2DRef Depth;
	FRITexture2DRef Normal;
	FRITexture2DRef Albedo;
	FRITexture2DRef MetallicRoughness;
	FRITexture2DRef Emissive;


	FRenderSystemGeom* Geometry;
	FRenderSystemSkinnedGeom* SkinnedGeometry;


	FRIUniformBuffer* TransformBuffer;
	FRIUniformBuffer* JointBuffer;
	FRIUniformBuffer* CameraMatrixBuffer;
	FRIUniformBuffer* EmissionPropertiesBuffer;

public:
	DRStageGBuffer() : FRenderStage("GBufferGen")
	{}

	void CreateResources(ShaderLibrary& library, FRIContext* context);
	void RecreateResources(ShaderLibrary& library, FRIContext* context, FRIContext* previousContext = NULL);

	void Prepare(FRICommandList& cmdList, RStageInterface& prev);
	void SubmitPass(FRICommandList& cmdList, Scene* scene);
	void Finish(FRICommandList& cmdList, RStageInterface& rso);
};



/*----------------------------------------------------------------*/

class DRStageShadowmap : public FRenderStage
{
public:
	FRIUniformBuffer* CascadeCameraBuffer;
	FViewportRect Viewport;


	FRIFrameBuffer* ShadowmapFrameBuffer;
	FRITexture2DArray* Output;

	FRIShaderPipeline* Shader;
	FRIShaderPipeline* SkinnedShader;

	FRenderSystemSM* SMGeometry;
	FRenderSystemSkinnedSM* SkinnedSMGeometry;

	FRIUniformBuffer* TransformBuffer;
	FRIUniformBuffer* JointBuffer;
	FRIUniformBuffer* CameraMatrixBuffer;


	DRStageShadowmap() : FRenderStage("Shadowmap")
	{}


	void CreateResources(ShaderLibrary& library, FRIContext* context);
	void RecreateResources(ShaderLibrary& library, FRIContext* context, FRIContext* previousContext = NULL);

	void Prepare(FRICommandList& cmdList, RStageInterface& input);
	void SubmitPass(FRICommandList& cmdList, Scene* scene);
	void Finish(FRICommandList& cmdList, RStageInterface& output);
};

/*-----------------------------------------------------------------*/

class HBAOPlus;

class DRStageAOGen : public FRenderStage
{
public:

	HBAOPlus* HbaoService;

	FRIFrameBuffer* FrameBuffer;
	FRITexture2D* AOTexture;

	FRITexture2D* DepthTexture;
	FRITexture2D* NormalTexture;

	DRStageAOGen() : FRenderStage("AOGen") {}

	void CreateResources(ShaderLibrary& library, FRIContext* context);
	void RecreateResources(ShaderLibrary& library, FRIContext* context, FRIContext* previousContext = NULL);

	void Prepare(FRICommandList& cmdList, RStageInterface& input);
	void SubmitPass(FRICommandList& cmdList, Scene* scene);
	void Finish(FRICommandList& cmdList, RStageInterface& output);
};

/*-------------------------------------------------------------------*/

class AtmosphereRenderer;

class DRStageLighting : public FRenderStage
{
public:
	FRIShaderPipeline* LightingShader;
	FRIShaderPipeline* TransluscencyCombineShader;

	FRIFrameBuffer* FrameBuffer;
	FRITexture2D* LitTexture;
	FRITexture2DRef BRDFLut;


	FRIUniformBuffer* CascadeCameraBuffer;
	FRIUniformBuffer* LightingConstantBuffer;
	FRIUniformBuffer* PointLightBuffer;

	FRIBlendState* BlendState;
	FRIRasterizerState* RasterizerState;
	FRIDepthStencilState* DepthStencilState;

	AtmosphereRenderer* Atmosphere;


	DRStageLighting() : FRenderStage("Lighting") {}


	void CreateResources(ShaderLibrary& library, FRIContext* context);
	void RecreateResources(ShaderLibrary& library, FRIContext* context, FRIContext* previousContext = NULL);

	void Prepare(FRICommandList& cmdList, RStageInterface& input);
	void SubmitPass(FRICommandList& cmdList, Scene* scene);
	void Finish(FRICommandList& cmdList, RStageInterface& output);
};


/*-------------------------------------------------------------------*/

class DRStageSMAA : public FRenderStage
{
public:

	FRIShaderPipeline* EdgeDetection;
	FRIShaderPipeline* BlendingWeight;
	FRIShaderPipeline* Neighbourhood;

	FRIFrameBuffer* EdgeFrameBuffer;
	FRIFrameBuffer* BlendFrameBuffer;
	FRIFrameBuffer* OutputFrameBuffer;

	FRITexture2D* EdgeTex;
	FRITexture2D* BlendTex;
	FRITexture2D* Output;


	FRITexture2D* SearchTex;
	FRITexture2D* AreaTex;


	DRStageSMAA() : FRenderStage("SMAA") {}

	void CreateResources(ShaderLibrary& library, FRIContext* context);
	void RecreateResources(ShaderLibrary& library, FRIContext* context, FRIContext* previousContext = NULL);


	void Prepare(FRICommandList& cmdList, RStageInterface& input);
	void SubmitPass(FRICommandList& cmdList, Scene* scene);
	void Finish(FRICommandList& cmdList, RStageInterface& output);

};


/*-------------------------------------------------------------------*/


class DRStagePostProcess : public FRenderStage
{
public:

	bool BloomEnabled = false;

	FRIShaderPipeline* PostProcessShader;

	FRIFrameBuffer* FrameBuffer;
	FRITexture2D* Output;

	DRStagePostProcess() : FRenderStage("PostProcess") {}

	void CreateResources(ShaderLibrary& library, FRIContext* context);
	void RecreateResources(ShaderLibrary& library, FRIContext* context, FRIContext* previousContext = NULL);


	void Prepare(FRICommandList& cmdList, RStageInterface& input);
	void SubmitPass(FRICommandList& cmdList, Scene* scene);
	void Finish(FRICommandList& cmdList, RStageInterface& output);

};


/*-------------------------------------------------------------------*/




class DRStageEnvironment : public FRenderStage
{
public:

	AtmosphereRenderer* Atmosphere;

	FRIFrameBuffer* FrameBuffer;
	FRITexture2D* Output;


	DRStageEnvironment() : FRenderStage("Environment") {}

	void CreateResources(ShaderLibrary& library, FRIContext* context);
	void RecreateResources(ShaderLibrary& library, FRIContext* context, FRIContext* previousContext = NULL);


	void Prepare(FRICommandList& cmdList, RStageInterface& input);
	void SubmitPass(FRICommandList& cmdList, Scene* scene);
	void Finish(FRICommandList& cmdList, RStageInterface& output);

};



class DRStageTransluscency : public FRenderStage
{
public:


	FRIFrameBuffer* FrameBuffer;
	FRITexture2D* Output;


	FRIShaderPipeline* ObjectPipeline;
	FRIUniformBuffer* TransformBuffer;
	FRIUniformBuffer* DirLightBuffer;

	DRStageTransluscency() : FRenderStage("Transluscency") {}

	void CreateResources(ShaderLibrary& library, FRIContext* context);
	void RecreateResources(ShaderLibrary& library, FRIContext* context, FRIContext* previousContext = NULL);


	void Prepare(FRICommandList& cmdList, RStageInterface& input);
	void SubmitPass(FRICommandList& cmdList, Scene* scene);
	void Finish(FRICommandList& cmdList, RStageInterface& output);

};
