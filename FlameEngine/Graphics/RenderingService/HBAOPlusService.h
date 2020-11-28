#pragma once

#include "../../Graphics/ShaderDefinitions/Shader.h"
#include "../../Graphics/FrameBuffer.h"
#include "../../Context/Context.h"
#include "../../Graphics/VertexBuffer.h"
#include "../../Graphics/Vertex.h"
#include "../../Graphics/RenderState.h"
#include "../../Graphics/Texture.h"

#include <GFSDK_SSAO.h>

EXPORT_CLASS HBAOPlusService
{
public:

	HBAOPlusService(Context* context);

	void RenderAO(Texture* depthTex, Texture* normTex, FrameBuffer* outputFBO, fMatrix4 projection, fMatrix4 view);


	GFSDK_SSAO_Parameters	mAoParams;
	GFSDK_SSAO_GLFunctions	mGLFunctions;
	GFSDK_SSAO_Context_GL* mHbaoGlContext;

	Context* mAttachedContext;
};

