#pragma once

#include "../../dll/nchfx.h"

#include "../ShaderDefinitions/Program.h"
#include "../Common/RenderBatch.h"
#include "../Common/FrameBuffer.h"
#include "../../Scene/Scene.h"
#include "../RenderingService/GaussianBlurService.h"
#include "../RenderingService/KawaseBlurService.h"
#include "../RenderingService/HBAOPlusService.h"
#include "../RenderingService/BoundingVolumeDebugService.h"



EXPORT(class,  DeferredRenderer)
{
public:
	DeferredRenderer(Context* context);

	void BeginRender(Scene* scene);
	void EndRender();


	bool enableHBAO = true;
	bool enableDEBUGTexture = true;
	bool enableKawaseBlur = true;
	int mBlurPassIndex = 1;
	bool enableGlossMap = true;

private:


	Program* mSceneDataShader;
	Program* mSsaoShader;
	Program* mCombineShader;


	Texture* mDepthBuffer;
	Texture* mNormalBuffer;
	Texture* mAlbedoBuffer;
	Texture* mSpecularBuffer;

	Texture* mSsaoBuffer;
	Texture* mShadowmapBuffer;

	FrameBuffer* mFrameBuffer;
	FrameBuffer* mSsaoFrameBuffer;
	FrameBuffer* mShadowmapFrameBuffer;

	VertexBuffer* mQuadBuffer;

	RenderState* mRenderState;
	FMatrix4 View;

	KawaseBlurService* mBlurRenderingService;
	HBAOPlusService* mHbaoPlusService;
	BoundingVolumeDebugService* mBoundingService;

	RenderBatch* _renderBatch;


	Context* attachedContext;

};

