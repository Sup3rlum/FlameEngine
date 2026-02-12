#pragma once


#include "Core/Engine/GameSystem/RenderObject.h"
#include "Core/Framework/IO/Stream.h"
#include "Core/Engine/ContentSystem/Client/LocalAssetManager.h"
#include "Core/Engine/ContentSystem/ImportScripts/Common.h"
#include "../Common/RenderUtil.h"

EXPORT(class, SkySphere) : public RenderList
{
public:
	void CreateResources(FRIContext* FriContext);
	void AddToCmdList(FRICommandList& cmdList, GRenderMode mode) override;
	void SetRenderStates(FRICommandList& cmdList);
	FRIShaderPipeline* Pipeline;
	FRIRasterizerState* AtmRasterizer;
	FRIDepthStencilState* AtmDepthState;
};