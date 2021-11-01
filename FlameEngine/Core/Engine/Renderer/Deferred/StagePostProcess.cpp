#include "RenderStages.h"


void DRStagePostProcess::CreateResources(ShaderLibrary& Shaders, FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());
	IVector2 viewportSize = renderContext->GetViewport().Size;


	Output = cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);

	FrameBuffer = cmdList.GetDynamic()->CreateFrameBuffer({ FRIFrameBufferAttachment(Output) }, true);
	PostProcessShader = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["PostProcess"]);


	BlendState = cmdList.GetDynamic()->CreateBlendState(EFRIAlphaBlend::Src, EFRIAlphaBlend::OneMinusSrc);
	DepthStencilState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);
	RasterizerState = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);
}

void DRStagePostProcess::RecreateResources(ShaderLibrary& Shaders, FRIContext* renderContext, FRIContext* prevContext)
{

}



void DRStagePostProcess::Prepare(FRICommandList& cmdList, RStageInterface& input)
{

	cmdList.SetBlendState(BlendState);
	cmdList.SetRasterizerState(RasterizerState);
	cmdList.GetDynamic()->SetDepthStencilState(DepthStencilState);


	cmdList.SetShaderSampler(FUniformSampler(0, input.GetResource<FRITexture2D>(RS_SLOT_AA)));
}
void DRStagePostProcess::SubmitPass(FRICommandList& cmdList, Scene* scene)
{
	//cmdList.ClearBuffer(FrameBuffer, Color::Black);
	cmdList.ClearBuffer(NULL, Color::Blue);
	{
		cmdList.SetShaderPipeline(PostProcessShader);
		FRenderUtil::DrawScreenQuad(cmdList);
	}
	cmdList.UnbindFrameBuffer();
}
void DRStagePostProcess::Finish(FRICommandList& cmdList, RStageInterface& rso)
{
	//rso.Buffer[RS_SLOT_FINAL] = Output;
}