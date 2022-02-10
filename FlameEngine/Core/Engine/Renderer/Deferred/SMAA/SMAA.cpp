#include "SMAA.h"

#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"
#include "Core/Engine/ContentSystem/Client/AssetImportScripts/ShaderLibrary.h"


#include "LUT.h"

void SMAA::CreateResources(FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());

	ShaderLibrary Shaders = FLocalContent::LoadFromLocal<ShaderLibrary>("shaders/smaa_dx.fslib", renderContext);


	// Create textures
	IVector2 viewportSize = renderContext->GetViewport().Size;
	EdgeTex = cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA32F);
	BlendTex = cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA32F);
	Output = cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA32F);



	SearchTex = cmdList.GetDynamic()->CreateTexture2D(
		SEARCHTEX_WIDTH, 
		SEARCHTEX_HEIGHT, 
		0, 
		EFRITextureFormat::R8UNORM, 
		FRIColorDataFormat(EFRIChannels::R, EFRIPixelStorage::UnsignedByte), 
		FRICreationDescriptor(searchTexBytes, SEARCHTEX_SIZE));

	AreaTex = cmdList.GetDynamic()->CreateTexture2D(
		AREATEX_WIDTH,
		AREATEX_HEIGHT,
		0,
		EFRITextureFormat::RG8UNORM,
		FRIColorDataFormat(EFRIChannels::RG, EFRIPixelStorage::UnsignedByte),
		FRICreationDescriptor(areaTexBytes, AREATEX_SIZE));



	EdgeFrameBuffer = cmdList.GetDynamic()->CreateFrameBuffer({ FRIFrameBufferAttachment(EdgeTex) }, true);
	BlendFrameBuffer = cmdList.GetDynamic()->CreateFrameBuffer({ FRIFrameBufferAttachment(BlendTex) }, true);
	OutputFrameBuffer = cmdList.GetDynamic()->CreateFrameBuffer({ FRIFrameBufferAttachment(Output) } , true);


	EdgeDetection = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["SMAAEdgeDetection"]);
	BlendingWeight = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["SMAABlendingWeight"]);
	Neighbourhood = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["SMAANeighbourhoodBlend"]);

	BlendState = cmdList.GetDynamic()->CreateBlendState(EFRIAlphaBlend::One, EFRIAlphaBlend::One);
	DepthStencilState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);
	RasterizerState = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);
}
void SMAA::RecreateResources(FRIContext* context, FRIContext* previousContext)
{

}

void SMAA::SubmitPass(FRICommandList& cmdList, FRITexture2D* input)
{

	cmdList.SetBlendState(BlendState);
	cmdList.SetRasterizerState(RasterizerState);
	cmdList.GetDynamic()->SetDepthStencilState(DepthStencilState);


	cmdList.SetShaderSampler(FUniformSampler(0, input));

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
		cmdList.SetShaderSampler(FUniformSampler(1, EdgeTex));
		cmdList.SetShaderSampler(FUniformSampler(2, AreaTex));
		cmdList.SetShaderSampler(FUniformSampler(3, SearchTex));

		FRenderUtil::DrawScreenQuad(cmdList);
	}
	cmdList.UnbindFrameBuffer();



	cmdList.ClearBuffer(OutputFrameBuffer, Color::Transparent);
	{
		cmdList.SetShaderPipeline(Neighbourhood);
		cmdList.SetShaderSampler(FUniformSampler(1, BlendTex));

		FRenderUtil::DrawScreenQuad(cmdList);
	}
	cmdList.UnbindFrameBuffer();
}

FRITexture2D* SMAA::GetOutput()
{
	return Output;
}