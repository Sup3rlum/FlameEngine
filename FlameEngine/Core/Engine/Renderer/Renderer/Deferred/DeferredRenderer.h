#pragma once

#include "Core/Common/CoreCommon.h"

#include "Core/Engine/ShaderFramework/Common/Program.h"
#include "../Common/FrameBuffer.h"
#include "../../Scene/Scene.h"
#include "../RenderingService/GaussianBlurService.h"
#include "../RenderingService/KawaseBlurService.h"
#include "../RenderingService/HBAOPlusService.h"
#include "../RenderingService/BoundingVolumeDebugService.h"

#include "../../Framework/PNG/PngHelper.h"

EXPORT(class,  DeferredRenderer)
{
public:
	DeferredRenderer(Context* context);

	void BeginRender(Scene* scene);
	void EndRender();


	void ScreenShot();

	bool enableHBAO = true;
	bool enableDEBUGTexture = true;
	bool enableKawaseBlur = true;
	int mBlurPassIndex = 1;
	bool enableGlossMap = true;

	FVector3 corn1[8];
	FVector3 corn2[8];
	FVector3 corn3[8];

private:


	Program* mSceneDataShader;
	Program* mSsaoShader;
	Program* mCombineShader;
	Program* mGPassShader;

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

