#pragma once


#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/Renderer/Common/RenderUtil.h"

#include "UXFRISurface.h"

class UXRenderer
{
public:

	void LoadResources(FRIContext* renderContext);
	void Render(FRICommandList& cmdList, UXFRISurface* service);

	FRIContext* FriContext;
	FRIDepthStencilState* DepthStencilState;
	FRIShaderPipeline* shader;
};