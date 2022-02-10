#pragma once

#include "Definitions.h"

#include "../Common/RenderUtil.h"
#include "../RenderSystems.h"


#define UBO_SLOT_CAMERA 0
#define UBO_SLOT_TRANSFORM 1
#define UBO_SLOT_CASCADE_CAMERA 2
#define UBO_SLOT_JOINTS 3
#define UBO_SLOT_COMBINE 4
#define UBO_SLOT_MATERIAL 5
#define UBO_SLOT_DLIGHT_DATA 6
#define UBO_SLOT_PLIGHT_DATA 7
#define UBO_SLOT_SLIGHT_DATA 8
#define UBO_SLOT_VXGI 10


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
#define RS_SLOT_RADIANCE 12
#define RS_SLOT_VOXEL_ANISO 13



struct FTransformBufferStruct
{
	FMatrix4 World;
	FMatrix4 WorldInverseTranspose;
};

struct FMaterialBufferStruct
{
	FVector4 Emissive;
	uint32 HasNormalMap;
	uint32 HasPOMEnabled;
	uint32 HasAOMap;
	uint32 HasEmissive;
	uint32 HasTransluscent;
};


class DRStageVXGI : public FRenderStage
{

	FRIShaderPipeline* voxelizePipeline;
	FRIShaderPipeline* lightCompute;
	FRIShaderPipeline* propagationCompute;
	FRIShaderPipeline* flushCompute;
	FRIShaderPipeline* anisoCompute;

	FRIUniformBuffer* CameraMatrixBuffer;
	FRIUniformBuffer* TransformBuffer;
	FRIUniformBuffer* MaterialBuffer;
	FRIUniformBuffer* VXGIBuffer;
	FRIUniformBuffer* PointLightBuffer;
	FRIUniformBuffer* DirLightBuffer;

	FRITexture3D* voxelAlbedo;
	FRITexture3D* voxelNormal;
	FRITexture3D* voxelEmission;
	FRITexture3D* voxelRadiance;
	FRITexture3D* voxelFirstBounce;

	FRITexture3D* DirectionalVolumes[6];

	FRIFrameBuffer* framebuffer;
	FRITexture2D* outputTex;

	FRIDepthStencilState* DepthStencilState;
	FRIRasterizerState* RasterizerState;
	FRIBlendState* BlendState;

	FMatrix4 voxelizeProjection;
	FViewportRect VoxelizeViewport;
	FViewportRect OriginalViewport;

	FRIDepthStencilState* FinalDepth;



public:
	bool NeedsUpdate = true;
	bool getFirstBounce = false;

	DRStageVXGI() : FRenderStage("VXGI")
	{}

	void CreateResources(ShaderLibrary& library, FRIContext* context);
	void RecreateResources(ShaderLibrary& library, FRIContext* context, FRIContext* previousContext = NULL);

	void Prepare(FRICommandList& cmdList, RStageInterface& prev);
	void SubmitPass(FRICommandList& cmdList, Scene* scene);
	void Finish(FRICommandList& cmdList, RStageInterface& rso);


	void GenerateMipMaps(FRICommandList& cmdList, FRITexture3D* voxelmap);
	void FlushVolumes(FRICommandList& cmdList);

	void InjectRadiance(FRICommandList& cmdList, Scene* scene);
	void InjectFirstBounce(FRICommandList& cmdList);

	void Voxelize(FRICommandList& cmdList, Scene* scene);
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

	FRITexture3D* voxelAlbedo;
	FRITexture3D* voxelNormal;
	FRITexture3D* voxelEmission;

	FRenderSystemGeom* Geometry;
	FRenderSystemSkinnedGeom* SkinnedGeometry;


	FRIUniformBuffer* TransformBuffer;
	FRIUniformBuffer* JointBuffer;
	FRIUniformBuffer* CameraMatrixBuffer;
	FRIUniformBuffer* MaterialBuffer;

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
	FRIUniformBuffer* DeferredConstantBuffer;

	FRIUniformBuffer* DirectionalLightBuffer;
	FRIUniformBuffer* PointLightBuffer;
	FRIUniformBuffer* SpotLightBuffer;

	FRIUniformBuffer* VXGIBuffer;

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

	void StageLightData(FRICommandList& cmdList, Scene* scene);

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
