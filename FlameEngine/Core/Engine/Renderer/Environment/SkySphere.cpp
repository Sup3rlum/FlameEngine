#include "SkySphere.h"


void SkySphere::CreateResources(FRIContext* FriContext)
{
	/*FRICommandList cmdList(FriContext->GetFRIDynamic());


	FAssetManager Content;
	Content.Connect("./Assets/");
	Content.RenderContext = FriContext;
	auto Shaders = Content.Load<ShaderLibrary>("Shaders/atmosphere_dx.fslib");

	Pipeline = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["AtmosphereBasic"]);
	AtmRasterizer = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);
	AtmDepthState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);
	//SettingsBuffer.GPU = cmdList.GetDynamic()->CreateConstantBuffer(sizeof(FAtmosphereConstantBuffer), EFRIAccess::Write, EFRIUsage::Dynamic);*/
}

void SkySphere::AddToCmdList(FRICommandList& cmdList, GRenderMode mode)
{
	if (mode != GRenderMode::PreLighting)
	{
		return;
	}

	SetRenderStates(cmdList);

	cmdList.SetShaderPipeline(Pipeline);
	FRenderUtil::DrawScreenQuad(cmdList);
}
void SkySphere::SetRenderStates(FRICommandList& cmdList)
{
	cmdList.SetRasterizerState(AtmRasterizer);
	cmdList.SetDepthStencilState(AtmDepthState);
}