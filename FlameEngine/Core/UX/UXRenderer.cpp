#include "UXRenderer.h"

#include "Core/Engine/ContentSystem/Client/LocalAssetManager.h"
#include "Core/Engine/ContentSystem/ImportScripts/ShaderLibrary.h"

void UXRenderer::LoadResources(FRIContext* renderContext)
{
	FriContext = renderContext;
	FRICommandList cmdList(renderContext->GetFRIDynamic());

	FAssetManager Content;
	Content.Connect("./Assets/");
	Content.RenderContext = renderContext;
	auto lib  = Content.Load<ShaderLibrary>("Shaders/ux_dx.fslib");

	shader = cmdList.GetDynamic()->CreateShaderPipeline(lib.Modules["UX"]);
	DepthStencilState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);
}

void UXRenderer::Render(FRICommandList& cmdList, UXFRISurface* surface)
{
	cmdList.GetDynamic()->SetDepthStencilState(DepthStencilState);
	cmdList.SetShaderPipeline(shader);
	cmdList.SetShaderSampler(FRISampler(0, surface->GetTexture()));

	FRenderUtil::DrawScreenQuad(cmdList);
}