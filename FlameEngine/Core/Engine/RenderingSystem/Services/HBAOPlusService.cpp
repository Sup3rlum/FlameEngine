#include "HBAOPlusService.h"

HBAOPlusService::HBAOPlusService(Context* c)
{
	mAttachedContext = c;


	GFSDK_SSAO_CustomHeap CustomHeap;
	CustomHeap.new_ = ::operator new;
	CustomHeap.delete_ = ::operator delete;

	GFSDK_SSAO_INIT_GL_FUNCTIONS(mGLFunctions);

	GFSDK_SSAO_Status status = GFSDK_SSAO_CreateContext_GL(&mHbaoGlContext, &mGLFunctions, &CustomHeap);
	if (status != GFSDK_SSAO_OK)
		FLAME_ERROR("FAILED TO INITIALIZE HBAO+!");


	GFSDK_SSAO_Version Version;
	status = GFSDK_SSAO_GetVersion(&Version);

	mAoParams.Radius = 0.6f;
	mAoParams.Bias = 0.5f;
	mAoParams.NearAO = 4.0f;
	mAoParams.FarAO = 1.5f;

	mAoParams.BackgroundAO.Enable = false;
	mAoParams.BackgroundAO.BackgroundViewDepth = 10.0f;

	mAoParams.ForegroundAO.Enable = false;
	mAoParams.ForegroundAO.ForegroundViewDepth = 1.0f;

	mAoParams.DepthStorage = GFSDK_SSAO_FP16_VIEW_DEPTHS;
	mAoParams.PowerExponent = 2.0f;
	mAoParams.DepthClampMode = false ? GFSDK_SSAO_CLAMP_TO_BORDER : GFSDK_SSAO_CLAMP_TO_EDGE;
	mAoParams.Blur.Enable = true;
	mAoParams.Blur.Sharpness = 16.0f;
	mAoParams.Blur.Radius = GFSDK_SSAO_BLUR_RADIUS_4;


}

void HBAOPlusService::RenderAO(Texture* depthTex, Texture* normTex, FrameBuffer* outputFBO, FMatrix4 Projection, FMatrix4 View)
{

	View[3] = FVector4(0, 0, 0, 1.0f);

	GFSDK_SSAO_InputData_GL Input;
	Input.DepthData.DepthTextureType = GFSDK_SSAO_HARDWARE_DEPTHS;
	Input.DepthData.FullResDepthTexture = GFSDK_SSAO_Texture_GL(GL_TEXTURE_2D, depthTex->GetHandle());
	Input.DepthData.ProjectionMatrix.Data = GFSDK_SSAO_Float4x4(&Projection[0].x);
	Input.DepthData.ProjectionMatrix.Layout = GFSDK_SSAO_ROW_MAJOR_ORDER;
	Input.DepthData.MetersToViewSpaceUnits = 1.0f;


	/*Input.NormalData.Enable = true;
	Input.NormalData.FullResNormalTexture = GFSDK_SSAO_Texture_GL(GL_TEXTURE_2D, normTex->_handle);
	Input.NormalData.WorldToViewMatrix.Data = GFSDK_SSAO_Float4x4(&View[0].x);
	Input.NormalData.WorldToViewMatrix.Layout = GFSDK_SSAO_ROW_MAJOR_ORDER;
	Input.NormalData.DecodeScale = -2.f;
	Input.NormalData.DecodeBias = 1.0f;*/

	GFSDK_SSAO_RenderMask RenderMask = GFSDK_SSAO_RENDER_AO;
	GFSDK_SSAO_Output_GL Output;
	Output.OutputFBO = outputFBO->_handle;
	Output.Blend.Mode = GFSDK_SSAO_OVERWRITE_RGB;

	GFSDK_SSAO_Status status;
	status = mHbaoGlContext->RenderAO(Input, mAoParams, Output, RenderMask);
}