#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"
#include "../Common/Geometry/VertexComponent.h"
#include "../../GameSystem/Common/Scene.h"


#include "../RenderSystems.h"
#include "../Common/RenderUtil.h"
#include "Core/Engine/ContentSystem/Client/AssetImportScripts/ShaderLibrary.h"






class DeferredRenderer;

EXPORT(class, VXGIRenderer) : public RenderModule
{
public:

	uint32 LightBounces = 2;
	bool NeedsUpdate = true;

	void CreateResources(FRIContext* renderContext);
	void RecreateResources(FRIContext* renderContext, FRIContext* prevContext = 0);


	void Render(FRICommandList& cmdList, DeferredRenderer* renderer);
	void VoxelizeScene(FRICommandList& cmdList, DeferredRenderer* renderer);


	FRIShaderPipeline* Voxelize;
	FRIShaderPipeline* LightingCompute;
	FRIShaderPipeline* PropagationCompute;
	FRIShaderPipeline* FlushCompute;
	FRIShaderPipeline* AnisoCompute;


	FRIUniformBuffer* VXGIBuffer;
	FRIUniformBuffer* PropagationData;
	FRIUniformBuffer* VoxelizeBuffer;


	FRITexture3D* VoxelAlbedo;
	FRITexture3D* VoxelNormal;
	FRITexture3D* VoxelEmission;
	FRITexture3D* VoxelBaseRadiance;
	FRITexture3D* VoxelFinal;
	FRITexture3D* VoxelAnisotropic[6];


	FRIFrameBuffer* OutputFBO;
	FRITexture2D* OutputTex;

	FRIDepthStencilState* DepthStencilState;
	FRIRasterizerState* RasterizerState;
	FRIBlendState* BlendState;

	FRIDepthStencilState* FinalDepth;


	FViewportRect VoxelizeViewport;
	FViewportRect OriginalViewport;



	void GenerateMipMaps(FRICommandList& cmdList, FRITexture3D* voxelmap);
	void FlushVolumes(FRICommandList& cmdList);

	void InjectRadiance(FRICommandList& cmdList);
	void InjectLightBounce(FRICommandList& cmdList, FRITexture3D* base, FRITexture3D* output, uint32 bounceNum);

};