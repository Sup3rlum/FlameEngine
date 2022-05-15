#pragma once

#include "Common/Geometry/VertexComponent.h"

#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Framework/IO/FileStream.h"
#include "RenderModule.h"
#include "Common/RenderUtil.h"

class BlurRenderer : public RenderModule
{
public:
	BlurRenderer();

	FRITexture2D* Render(FRICommandList& cmdList, FRITexture2D* texture);
	void CreateResources(FRIContext* cmdList);
	void RecreateResources(FRIContext* cmdList, FRIContext* previous);

	FRIShaderPipelineRef gaussian[2];
	FRIShaderPipelineRef passthrough;
	FRIShaderPipelineRef additive;

	FRITexture2D* textures[3];
	FRIFrameBuffer* framebuffers[3];

	FViewportRect viewportRect;
	FViewportRect contextViewport;
};
