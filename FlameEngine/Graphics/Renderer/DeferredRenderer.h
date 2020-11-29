#pragma once

#include "../../dll/nchfx.h"

#include "../ShaderDefinitions/Shader.h"
#include "../Common/RenderBatch.h"
#include "../Common/FrameBuffer.h"
#include "../../Scene/Scene.h"
#include "../RenderingService/GaussianBlurService.h"
#include "../RenderingService/KawaseBlurService.h"
#include "../RenderingService/HBAOPlusService.h"


EXPORT_ENUM SSAOStrength
{
	SAMPLES_1 = 1,
	SAMPLES_2 = 2,
	SAMPLES_4 = 4,
	SAMPLES_8 = 8,
	SAMPLES_16 = 16,
	SAMPLES_24 = 24,
	SAMPLES_36 = 36,
	SAMPLES_48 = 48,
	SAMPLES_64 = 64,
	SAMPLES_128 = 128
};

EXPORT(class,  DeferredRenderer)
{
public:
	DeferredRenderer(Context* context);

	void BeginRender(Scene* scene);
	void EndRender();

	void SetSSAOSamples(SSAOStrength s);


	bool enableHBAO = true;
	bool enableDEBUGTexture = true;
	bool enableKawaseBlur = true;
	int mBlurPassIndex = 1;

private:



	SSAOStrength SSAOSampleSize = SSAOStrength::SAMPLES_16;


	Shader* mSceneDataShader;
	Shader* mSsaoShader;
	Shader* mCombineShader;


	Texture* mDepthBuffer;
	Texture* mNormalBuffer;
	Texture* mAlbedoBuffer;
	Texture* mSsaoBuffer;
	Texture* mShadowmapBuffer;

	FrameBuffer* mFrameBuffer;
	FrameBuffer* mSsaoFrameBuffer;
	FrameBuffer* mShadowmapFrameBuffer;

	VertexBuffer* mQuadBuffer;

	RenderState* mRenderState;
	fMatrix4 View;

	KawaseBlurService* mBlurRenderingService;
	HBAOPlusService* mHbaoPlusService;

	Context* attachedContext;

};

