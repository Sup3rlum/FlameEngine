#pragma once


#include "../HBAOPlusInterface.h"

#include "Core/Engine/FlameRI/OpenGL/OpenGLFRI.h"
#include <GFSDK_SSAO.h>


class GLHBAOPlus : public HBAOPlus
{
public:

	GLHBAOPlus(FHBAOParameters params);
	void RenderAO(FRITexture2D* depthTex, FRITexture2D* normTex, FRIFrameBuffer* outputFBO, const FMatrix4& projection, const FMatrix4& view);

	/*GFSDK_SSAO_Parameters	mAoParams;
	GFSDK_SSAO_GLFunctions	mGLFunctions;
	GFSDK_SSAO_Context_GL* mHbaoGlContext;*/
};