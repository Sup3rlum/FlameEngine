#pragma once



#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"
#include "../../Common/Geometry/VertexComponent.h"
#include "../../../GameSystem/Common/Scene.h"


#include "../../RenderSystems.h"
#include "../../Common/RenderUtil.h"
#include "Core/Engine/ContentSystem/Client/AssetImportScripts/ShaderLibrary.h"





EXPORT(class, SMAA) : public RenderModule
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

	FRIDepthStencilState* DepthStencilState;
	FRIRasterizerState* RasterizerState;
	FRIBlendState* BlendState;


	SMAA() {}

	void CreateResources(FRIContext* context);
	void RecreateResources(FRIContext* context, FRIContext* previousContext = NULL);

	void SubmitPass(FRICommandList& cmdList, FRITexture2D* input);

	FRITexture2D* GetOutput();

};