#include "UXRenderer.h"

#include "Core/Engine/ContentSystem/Client/LocalAssetManager.h"
#include "Core/Engine/ContentSystem/ImportScripts/ShaderLibrary.h"

void UXRenderer::LoadResources(FRIContext* renderContext)
{
	FriContext = renderContext;
	FRICommandList cmdList(renderContext->GetCommandContext(0));
	cmdList.Open();

	auto Allocator = renderContext->GetFRIDynamic();

	FAssetManager Content;
	Content.Connect("./Assets/");
	Content.RenderContext = renderContext;
	auto lib = Content.Load<ShaderLibrary>("Shaders/ux_dx.fslib");


	FRIDescriptorRange Texture0Range;
	Texture0Range.NumDescriptors = 1;
	Texture0Range.BaseShaderRegister = 0;
	Texture0Range.RegisterSpace = 0;
	Texture0Range.OffsetInDescriptorsFromTableStart = 0xffffffff;
	Texture0Range.RangeType = EFRIRootDescriptorRangeType::SRV;

	FRIRootDescriptorTable Textures;
	Textures.NumRanges = 1;
	Textures.Ranges = &Texture0Range;


	FArray<FRIStaticSampler> StaticSamplers;
	StaticSamplers.Add(FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 0, 0));

	// Input Layout
	FRIInputLayout StaticLitLayout = {
	.NumElements = 2,
	.DeclarationElements = new FRIInputElementDesc[2]
	{
		FRIInputElementDesc("POSITION", EFRIAttributeType::Float3, 0, EFRIAttribUsage::PerVertex),
		FRIInputElementDesc("TEXCOORD", EFRIAttributeType::Float2, 1, EFRIAttribUsage::PerVertex),
	}
	};

	FRIRasterizerState Rasterizer{ EFRICullMode::Back , EFRIFillMode::Solid };
	FRIDepthStencilState Depth{ EFRIBool::False };
	FRIBlendState Blend{ EFRIBlend::Src, EFRIBlend::OneMinusSrc };

	pso = Allocator->CreatePipelineStateObject(
		lib.Modules["UX"],
		StaticLitLayout,
		{ FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Pixel, Textures) }, 
		StaticSamplers, 
		{ EFRITextureFormat::RGB8UNORM }, 
		Rasterizer,
		Depth,
		Blend);

	DepthStencilState = Allocator->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);

	cmdList.CloseAndExecute();
}

void UXRenderer::Render(FRICommandList& cmdList, UXFRISurface* surface)
{
	cmdList.SetDepthStencilState(DepthStencilState);
	cmdList.SetPipelineState(pso);
	cmdList.SetShaderResourceViewPS(0, surface->GetView());

	FRenderUtil::DrawScreenQuad(cmdList);
}