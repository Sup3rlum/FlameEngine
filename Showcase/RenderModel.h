#pragma once

#include <FlameEngine/Core/Engine/GameSystem/RenderObject.h>
#include "FlameEngine/Core/Engine/ContentSystem/ImportScripts/Common.h"
#include <FlameEngine/Core/Framework/IO/Stream.h>
#include <FlameEngine/Core/Engine/ContentSystem/Client/LocalAssetManager.h>

struct ModelRenderObject : public RenderList
{
	Model model;
	FRIContext* FriContext;

	FRIShaderPipeline* dissolveShader;
	FRITexture2D* dissolveMask;
	FRIStageBuffer dissolveBuffer;

	FRIRasterizerState* rasterizer;

	ModelRenderObject(FRIContext* FriContext);
	void AddToCmdList(FRICommandList& cmdList, GRenderMode renderMode) override;
};