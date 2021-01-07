#pragma once

#include "../../Graphics/ShaderDefinitions/Shader.h"
#include "../../Graphics/Common/FrameBuffer.h"
#include "../../Context/Context.h"
#include "../../Graphics/Common/VertexBuffer.h"
#include "../../Graphics/Common/Vertex.h"
#include "../../Graphics/Common/RenderState.h"
#include "../../Graphics/Common/Texture.h"

#include <GFSDK_SSAO.h>

EXPORT(class,  HBAOPlusService)
{
public:

	HBAOPlusService(Context* context);

	void RenderAO(Texture* depthTex, Texture* normTex, FrameBuffer* outputFBO, FMatrix4 projection, FMatrix4 view);


	GFSDK_SSAO_Parameters	mAoParams;
	GFSDK_SSAO_GLFunctions	mGLFunctions;
	GFSDK_SSAO_Context_GL* mHbaoGlContext;

	Context* mAttachedContext;
};

