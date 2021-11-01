#include "RenderStages.h"


#include "../Services/HBAOPlus/HBAOPlusInterface.h"


void DRStageAOGen::CreateResources(ShaderLibrary& Shaders, FRIContext* renderContext)
{
	// Create Resources for Stage
	FRICommandList cmdList(renderContext->GetFRIDynamic());

	IVector2 viewportSize = renderContext->GetViewport().Size;
	AOTexture = cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);

	FArray<FRIFrameBufferAttachment> hbaoAttachment;
	hbaoAttachment.Add(FRIFrameBufferAttachment(AOTexture));

	FrameBuffer = cmdList.GetDynamic()->CreateFrameBuffer(hbaoAttachment, true);


	// Allocate HBAO parameters

	FHBAOParameters hbaoParams;

	hbaoParams.Radius = 1.0f;
	hbaoParams.Bias = 0.3f;
	hbaoParams.NearAO = 4.0f;
	hbaoParams.FarAO = 1.5f;

	hbaoParams.BackgroundAOEnable = false;
	hbaoParams.BackgroundAO_ViewDepth = 10.0f;

	hbaoParams.ForegroundAOEnable = false;
	hbaoParams.ForegroundAO_ViewDepth = 1.0f;

	hbaoParams.PowerExponent = 2.0f;
	hbaoParams.BlurEnable = true;
	hbaoParams.BlurSharpness = 16.0f;

	HbaoService = HBAOPlus::Allocate(renderContext, hbaoParams);
}

void DRStageAOGen::RecreateResources(ShaderLibrary& Shaders, FRIContext* context, FRIContext* previousContext)
{
}


void DRStageAOGen::Prepare(FRICommandList& cmdList, RStageInterface& input)
{
	DepthTexture = static_cast<FRITexture2D*>(input.Buffer[RS_SLOT_DEPTH]);
	NormalTexture = static_cast<FRITexture2D*>(input.Buffer[RS_SLOT_NORMAL]);

}
void DRStageAOGen::SubmitPass(FRICommandList& cmdList, Scene* scene)
{
	CameraComponent& CameraRef = scene->Camera.Component<CameraComponent>();

	HbaoService->RenderAO(DepthTexture, NormalTexture, FrameBuffer, CameraRef.Projection, CameraRef.View);
}
void DRStageAOGen::Finish(FRICommandList& cmdList, RStageInterface& rso)
{
	rso.Buffer[RS_SLOT_AO] = AOTexture;
}