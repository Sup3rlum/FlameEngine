#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Common/RenderUtil.h"
#include "Core/Framework/IO/FileStream.h"

class SATGenerator
{
public:
	void CreateResources(FRIContext* renderContext);


	void SetupContext(FVector2 textureSize, uint32 numLayers);

	void SwapTargets();
	void RenderPass(FRICommandList& cmdList, FVector2 direction, float passIndex);

	void Render(FRICommandList& cmdList, FRITexture2DArray* textureArray, FRIFrameBuffer* attachedFBO);


	FRIFrameBuffer* attachedFBO;
	FRIFrameBuffer* backFBO;
	FRITexture2DArray* textureRefA;
	FRITexture2DArray* textureRefB;

	FVector2 texSize;
	FVector2 passDimensions;


	uint32 arraylayers;

	FRIShaderPipeline* pipeline;

	uint32 PassIndexLoc;
	uint32 DirectionLoc;
	uint32 LayerIndexLoc;
};