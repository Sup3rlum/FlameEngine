#include "SMAA.h"

#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"
#include "Core/Engine/ContentSystem/Client/LocalAssetManager.h"
#include "Core/Engine/ContentSystem/ImportScripts/ShaderLibrary.h"


#include "LUT.h"

void SMAA::CreateResources(FRIContext* renderContext)
{
	/*FRICommandList cmdList(renderContext->GetFRIDynamic());

	FAssetManager Content;
	Content.Connect("./Assets/");	
	Content.RenderContext = renderContext;
	ShaderLibrary Shaders = Content.Load<ShaderLibrary>("Shaders/smaa_dx.fslib");


	// Create textures
	IVector2 viewportSize = renderContext->GetViewport().Size;
	EdgeTex = cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 1, EFRIAccess::None, EFRITextureFormat::RGBA32F);
	BlendTex = cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 1, EFRIAccess::None, EFRITextureFormat::RGBA32F);
	Output = cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 1, EFRIAccess::None, EFRITextureFormat::RGBA32F);



	SearchTex = cmdList.GetDynamic()->CreateTexture2D(
		SEARCHTEX_WIDTH, 
		SEARCHTEX_HEIGHT, 
		1, 
		EFRIAccess::None,
		EFRITextureFormat::R8UNORM, 
		new FRICreationDescriptor(searchTexBytes, SEARCHTEX_SIZE));

	AreaTex = cmdList.GetDynamic()->CreateTexture2D(
		AREATEX_WIDTH,
		AREATEX_HEIGHT,
		1,
		EFRIAccess::None,
		EFRITextureFormat::RG8UNORM,
		new FRICreationDescriptor(areaTexBytes, AREATEX_SIZE));



	EdgeFrameBuffer = cmdList.GetDynamic()->CreateFrameBuffer({ FRIFrameBufferAttachment(EdgeTex) }, true);
	BlendFrameBuffer = cmdList.GetDynamic()->CreateFrameBuffer({ FRIFrameBufferAttachment(BlendTex) }, true);
	OutputFrameBuffer = cmdList.GetDynamic()->CreateFrameBuffer({ FRIFrameBufferAttachment(Output) } , true);


	EdgeDetection = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["SMAAEdgeDetection"]);
	BlendingWeight = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["SMAABlendingWeight"]);
	Neighbourhood = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["SMAANeighbourhoodBlend"]);

	BlendState = cmdList.GetDynamic()->CreateBlendState(EFRIBlend::One, EFRIBlend::One, EFRIBlend::Src, EFRIBlend::Dst);
	DepthStencilState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);
	RasterizerState = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);*/
}
void SMAA::RecreateResources(FRIContext* context, FRIContext* previousContext)
{

}

void SMAA::SubmitPass(FRICommandList& cmdList, FRITexture2D* input)
{
	/*cmdList.SetBlendState(BlendState);
	cmdList.SetRasterizerState(RasterizerState);
	cmdList.SetDepthStencilState(DepthStencilState);

	cmdList.SetShaderResource(0, input->View());

	// Edge Detection
	cmdList.ClearBuffer(EdgeFrameBuffer, Color::Transparent);
	{
		cmdList.SetShaderPipeline(EdgeDetection);
		FRenderUtil::DrawScreenQuad(cmdList);
	}
	cmdList.UnbindFrameBuffer();



	cmdList.ClearBuffer(BlendFrameBuffer, Color::Transparent);
	{
		cmdList.SetShaderPipeline(BlendingWeight);
		cmdList.SetShaderResource(1, EdgeTex->View());
		cmdList.SetShaderResource(2, AreaTex->View());
		cmdList.SetShaderResource(3, SearchTex->View());

		FRenderUtil::DrawScreenQuad(cmdList);
	}
	cmdList.UnbindFrameBuffer();



	cmdList.ClearBuffer(OutputFrameBuffer, Color::Transparent);
	{
		cmdList.SetShaderPipeline(Neighbourhood);
		cmdList.SetShaderResource(1, BlendTex->View());

		FRenderUtil::DrawScreenQuad(cmdList);
	}
	cmdList.UnbindFrameBuffer();*/
}

FRITexture2D* SMAA::GetOutput()
{
	return Output;
}