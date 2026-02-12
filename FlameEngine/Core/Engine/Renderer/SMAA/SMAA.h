#pragma once



#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"
#include "../Common/Geometry/VertexComponent.h"
#include "../../GameSystem/Common/Scene.h"

#include "../Common/RenderUtil.h"
#include "Core/Engine/ContentSystem/ImportScripts/ShaderLibrary.h"



EXPORT(class, SMAA)
{
public:

	FRIShaderPipeline* EdgeDetection;
	FRIShaderPipeline* BlendingWeight;
	FRIShaderPipeline* Neighbourhood;

	FRIRenderTargetView* EdgeFrameBuffer;
	FRIRenderTargetView* BlendFrameBuffer;
	FRIRenderTargetView* OutputFrameBuffer;

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