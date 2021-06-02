#include "D3D11HBAOPlus.h"



D3D11HBAOPlus::D3D11HBAOPlus(FHBAOParameters params, ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext) :
	HBAOPlus(params),
	d3dDevice(d3dDevice),
	d3dDeviceContext(d3dDeviceContext)
{

	GFSDK_SSAO_CustomHeap CustomHeap;
	CustomHeap.new_ = ::operator new;
	CustomHeap.delete_ = ::operator delete;


	GFSDK_SSAO_Status status = GFSDK_SSAO_CreateContext_D3D11(d3dDevice, &mHbaoD3D11Context, &CustomHeap);

	/*if (status != GFSDK_SSAO_OK)
		FLAME_ERROR("FAILED TO INITIALIZE HBAO+!");*/

	GFSDK_SSAO_Version Version;
	status = GFSDK_SSAO_GetVersion(&Version);

	mAoParams.Radius = params.Radius;
	mAoParams.Bias = params.Bias;

	mAoParams.BackgroundAO.Enable = params.BackgroundAOEnable;
	mAoParams.BackgroundAO.BackgroundViewDepth = params.BackgroundAO_ViewDepth;

	mAoParams.ForegroundAO.Enable = params.ForegroundAOEnable;
	mAoParams.ForegroundAO.ForegroundViewDepth = params.ForegroundAO_ViewDepth;

	mAoParams.DepthStorage = GFSDK_SSAO_FP32_VIEW_DEPTHS;
	mAoParams.PowerExponent = params.PowerExponent;
	mAoParams.DepthClampMode = false ? GFSDK_SSAO_CLAMP_TO_BORDER : GFSDK_SSAO_CLAMP_TO_EDGE;
	mAoParams.Blur.Enable = params.BlurEnable;
	mAoParams.Blur.Sharpness = params.BlurSharpness;
	mAoParams.Blur.Radius = GFSDK_SSAO_BLUR_RADIUS_4;

}


void D3D11HBAOPlus::RenderAO(FRITexture2D* depthTex, FRITexture2D* normTex, FRIFrameBuffer* outputFBO, const FMatrix4& Projection, const FMatrix4& View)
{

	//View[3] = FVector4(0, 0, 0, 1.0f);

	GFSDK_SSAO_InputData_D3D11 Input;
	Input.DepthData.DepthTextureType = GFSDK_SSAO_HARDWARE_DEPTHS;
	Input.DepthData.pFullResDepthTextureSRV = static_cast<FD3D11Texture2D*>(depthTex)->ShaderResourceView;
	//Input.DepthData.pFullResDepthTextureSRV = static_cast<FD3D11FrameBuffer*>(outputFBO)->DSVShaderResourceView;
	Input.DepthData.ProjectionMatrix.Data = GFSDK_SSAO_Float4x4(&Projection[0].x);
	Input.DepthData.ProjectionMatrix.Layout = GFSDK_SSAO_ROW_MAJOR_ORDER;
	Input.DepthData.MetersToViewSpaceUnits = 1.0f;


	/*
	Input.NormalData.Enable = true;
	Input.NormalData.FullResNormalTexture = GFSDK_SSAO_Texture_GL(GL_TEXTURE_2D, ((FGLResourceTexture2D*)normTex)->textureHandle);
	Input.NormalData.WorldToViewMatrix.Data = GFSDK_SSAO_Float4x4(&View[0].x);
	Input.NormalData.WorldToViewMatrix.Layout = GFSDK_SSAO_ROW_MAJOR_ORDER;
	Input.NormalData.DecodeScale = 2.f;
	Input.NormalData.DecodeBias = -1.0f;*/

	GFSDK_SSAO_RenderMask RenderMask = GFSDK_SSAO_RENDER_AO;
	GFSDK_SSAO_Output_D3D11 Output;
	Output.pRenderTargetView = static_cast<FD3D11FrameBuffer*>(outputFBO)->RenderTargetViews[0];
	Output.Blend.Mode = GFSDK_SSAO_OVERWRITE_RGB;

	GFSDK_SSAO_Status status;
	status = mHbaoD3D11Context->RenderAO(d3dDeviceContext, Input, mAoParams, Output, RenderMask);

	assert(status == GFSDK_SSAO_OK);


}