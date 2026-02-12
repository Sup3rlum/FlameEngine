#include "BlurRenderer.h"

#include "Core/Engine/ContentSystem/ImportScripts/ShaderLibrary.h"
#include "Core/Engine/ContentSystem/Client/LocalAssetManager.h"

BlurRenderer::BlurRenderer()
{
}

void BlurRenderer::CreateResources(FRIContext* context)
{

	/*
	FRICommandList cmdList(context->GetFRIDynamic());

	FAssetManager Content;
	Content.Connect("./Assets/");
	Content.RenderContext = context;
	Shaders = Content.Load<ShaderLibrary>("Shaders/blur.fslib");

	gaussian[0] = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["GaussianBlurVertical"]);
	gaussian[1] = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["GaussianBlurHorizontal"]);
	passthrough = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["Passthrough"]);
	additive = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["Additive"]);

	contextViewport = context->GetViewport();
	viewportRect.Size = contextViewport.Size / 2.0f;

	textures[0] = cmdList.GetDynamic()->CreateTexture2D(viewportRect.Width, viewportRect.Height, 1, EFRIAccess::None, EFRITextureFormat::RGBA8UNORM);
	textures[1] = cmdList.GetDynamic()->CreateTexture2D(viewportRect.Width, viewportRect.Height, 1, EFRIAccess::None, EFRITextureFormat::RGBA8UNORM);
	textures[2] = cmdList.GetDynamic()->CreateTexture2D(contextViewport.Width, contextViewport.Height, 1, EFRIAccess::None, EFRITextureFormat::RGBA8UNORM);

	framebuffers[0] = cmdList.GetDynamic()->CreateFrameBuffer({ textures[0] }, false);
	framebuffers[1] = cmdList.GetDynamic()->CreateFrameBuffer({ textures[1] }, false);
	framebuffers[2] = cmdList.GetDynamic()->CreateFrameBuffer({ textures[2] }, false);*/

}

void BlurRenderer::RecreateResources(FRIContext* context, FRIContext* context2)
{
}

FRITexture2D* BlurRenderer::Render(FRICommandList& cmdList, FRITexture2D* Texture)
{

	/*
	// Resize to half size
	cmdList.SetViewport(viewportRect);

	cmdList.ClearBuffer(framebuffers[1], Color::Black);
	{
		cmdList.SetShaderPipeline(passthrough);
		cmdList.SetShaderResource(0, Texture->View());
		FRenderUtil::DrawScreenQuad(cmdList);
	}
	cmdList.UnbindFrameBuffer();
	cmdList.SetShaderResource(0, NULL);

	// Vertical Pass
	cmdList.ClearBuffer(framebuffers[0], Color::Black);
	{
		cmdList.SetShaderPipeline(gaussian[0]);
		cmdList.SetShaderResource(0, textures[1]->View());
		FRenderUtil::DrawScreenQuad(cmdList);
	}
	cmdList.UnbindFrameBuffer();
	cmdList.SetShaderResource(0, NULL);

	// Horizontal Pass
	cmdList.ClearBuffer(framebuffers[1], Color::Black);
	{
		cmdList.SetShaderPipeline(gaussian[1]);
		cmdList.SetShaderResource(0, textures[0]->View());
		FRenderUtil::DrawScreenQuad(cmdList);
	}
	cmdList.UnbindFrameBuffer();
	cmdList.SetShaderResource(0, NULL);

	// Resize to full res
	cmdList.SetViewport(contextViewport);

	cmdList.ClearBuffer(framebuffers[2], Color::Black);
	{
		cmdList.SetShaderPipeline(additive);
		cmdList.SetShaderResource(0, textures[1]->View());
		cmdList.SetShaderResource(1, Texture->View());
		FRenderUtil::DrawScreenQuad(cmdList);
	}
	cmdList.UnbindFrameBuffer();
	cmdList.SetShaderResource(0, NULL);
	cmdList.SetShaderResource(1, NULL);
	*/
	return NULL;// textures[2];
}