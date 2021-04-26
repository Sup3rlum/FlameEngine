#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Common/Viewport.h"
#include "Common/RenderUtil.h"
#include "Core/Framework/IO/FileStream.h"

class SATGenerator
{
public:
	void CreateResources(FRIContext* renderContext);


	void SetupContext(FVector2 textureSize, uint32 numLayers);

	void SwapTargets();
	void RenderPass(FRICommandList& cmdList, FVector2 direction, float passIndex);

	void Render(FRICommandList& cmdList, FResourceTexture2DArray* textureArray, FResourceFrameBuffer* attachedFBO);


	FResourceFrameBuffer* attachedFBO;
	FResourceFrameBuffer* backFBO;
	FResourceTexture2DArray* textureRefA;
	FResourceTexture2DArray* textureRefB;

	FVector2 texSize;
	FVector2 passDimensions;


	uint32 arraylayers;

	FResourceShaderPipeline* pipeline;

	uint32 PassIndexLoc;
	uint32 DirectionLoc;
	uint32 LayerIndexLoc;
};