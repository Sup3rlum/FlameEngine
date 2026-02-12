#include "DeferredRenderer.h"

#include "Core/Framework/IO/FileStream.h"
#include "Core/Engine/ContentSystem/Client/LocalAssetManager.h"
#include "../../ContentSystem/ImportScripts/Material.h"
#include "../../GameSystem/RenderObject.h"

#include "BRDF.h"
#include "../../Common/FTime.h"



const int32 CAMERA_CBUFFER_SIZE = sizeof(Camera) * 2;
const int32 TRANSFORM_CBUFFER_SIZE = sizeof(FMatrix4) * 4;
/*
CBuffers.CameraMatrix = Allocator->CreateConstantBuffer(sizeof(Camera) * 2, EFRIAccess::Write, EFRIUsage::Dynamic);
CBuffers.Transform = Allocator->CreateConstantBuffer(sizeof(FMatrix4) * 2, EFRIAccess::Write, EFRIUsage::Dynamic);
CBuffers.JointData = Allocator->CreateConstantBuffer(sizeof(FMatrix4) * 128, EFRIAccess::Write, EFRIUsage::Dynamic);
CBuffers.DLight = Allocator->CreateConstantBuffer(DirectionalLight::GetStageMemorySize() * 8, EFRIAccess::Write, EFRIUsage::Dynamic);
CBuffers.PLight = Allocator->CreateConstantBuffer(PointLight::GetStageMemorySize() * 8, EFRIAccess::Write, EFRIUsage::Dynamic);
CBuffers.SLight = Allocator->CreateConstantBuffer(SpotLight::GetStageMemorySize() * 8, EFRIAccess::Write, EFRIUsage::Dynamic);
CBuffers.LightingConstants = Allocator->CreateConstantBuffer(76 * sizeof(float), EFRIAccess::Write, EFRIUsage::Dynamic);
*/


FRIPipelineStateObject* CreateGeometryPipelineState(FRIDynamicAllocator* Allocator, const ShaderLibraryModule& Shaders, const FArray<EFRITextureFormat>& RTVFormats)
{
	FRIInputLayout StaticLitLayout = {
		.NumElements = 5,
		.DeclarationElements = new FRIInputElementDesc[5]
		{
			FRIInputElementDesc("POSITION", EFRIAttributeType::Float3, 0, EFRIAttribUsage::PerVertex),
			FRIInputElementDesc("NORMAL", EFRIAttributeType::Float3, 1, EFRIAttribUsage::PerVertex),
			FRIInputElementDesc("TANGENT", EFRIAttributeType::Float3, 2, EFRIAttribUsage::PerVertex),
			FRIInputElementDesc("BITANGENT", EFRIAttributeType::Float3, 3, EFRIAttribUsage::PerVertex),
			FRIInputElementDesc("TEXCOORD", EFRIAttributeType::Float2, 4, EFRIAttribUsage::PerVertex)
		}
	};

	// Texture 0

	FRIDescriptorRange Texture0Range;
	Texture0Range.NumDescriptors = 4;
	Texture0Range.BaseShaderRegister = 0;
	Texture0Range.RegisterSpace = 0;
	Texture0Range.OffsetInDescriptorsFromTableStart = 0xffffffff;
	Texture0Range.RangeType = EFRIRootDescriptorRangeType::SRV;

	FRIRootDescriptorTable Textures;
	Textures.NumRanges = 1;
	Textures.Ranges = &Texture0Range;

	// CBV

	FRIDescriptorRange ConstantBuffer1Range;
	ConstantBuffer1Range.NumDescriptors = 2;
	ConstantBuffer1Range.BaseShaderRegister = 0;
	ConstantBuffer1Range.RegisterSpace = 0;
	ConstantBuffer1Range.OffsetInDescriptorsFromTableStart = 0xffffffff;
	ConstantBuffer1Range.RangeType = EFRIRootDescriptorRangeType::CBV;

	FRIRootDescriptorTable ConstantBufferTable;
	ConstantBufferTable.NumRanges = 1;
	ConstantBufferTable.Ranges = &ConstantBuffer1Range;

	FArray<FRIRootParameter> RootParams = {
			FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Vertex, ConstantBufferTable),
			FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Pixel, ConstantBufferTable),
			FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Pixel, Textures),
	};
	FArray<FRIStaticSampler> StaticSamplers = {
			FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 0, 0),
			FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 1, 0),
			FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 2, 0),
			FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 3, 0),
	};


	FRIRasterizerState Rasterizer{ EFRICullMode::Back , EFRIFillMode::Solid };
	FRIDepthStencilState Depth{ EFRIBool::True };
	FRIBlendState Blend{ EFRIBlend::One, EFRIBlend::Zero };

	return Allocator->CreatePipelineStateObject(
		Shaders,
		StaticLitLayout,
		RootParams,
		StaticSamplers,
		RTVFormats,
		Rasterizer,
		Depth,
		Blend
	);

}

FRIPipelineStateObject* CreateGeometrySkinnedPipelineState(FRIDynamicAllocator* Allocator, const ShaderLibraryModule& Shaders, const FArray<EFRITextureFormat>& RTVFormats)
{
	FRIInputLayout StaticLitLayout = {
		.NumElements = 7,
		.DeclarationElements = new FRIInputElementDesc[7]
		{
			FRIInputElementDesc("POSITION", EFRIAttributeType::Float3, 0, EFRIAttribUsage::PerVertex),
			FRIInputElementDesc("NORMAL", EFRIAttributeType::Float3, 1, EFRIAttribUsage::PerVertex),
			FRIInputElementDesc("TANGENT", EFRIAttributeType::Float3, 2, EFRIAttribUsage::PerVertex),
			FRIInputElementDesc("BITANGENT", EFRIAttributeType::Float3, 3, EFRIAttribUsage::PerVertex),
			FRIInputElementDesc("TEXCOORD", EFRIAttributeType::Float2, 4, EFRIAttribUsage::PerVertex),
			FRIInputElementDesc("BLENDINDICES", EFRIAttributeType::Int4, 5, EFRIAttribUsage::PerVertex),
			FRIInputElementDesc("BLENDWEIGHTS", EFRIAttributeType::Float4, 6, EFRIAttribUsage::PerVertex),
		}
	};

	// Texture 0

	FRIDescriptorRange Texture0Range;
	Texture0Range.NumDescriptors = 4;
	Texture0Range.BaseShaderRegister = 0;
	Texture0Range.RegisterSpace = 0;
	Texture0Range.OffsetInDescriptorsFromTableStart = 0xffffffff;
	Texture0Range.RangeType = EFRIRootDescriptorRangeType::SRV;

	FRIRootDescriptorTable Textures;
	Textures.NumRanges = 1;
	Textures.Ranges = &Texture0Range;

	// CBV

	FRIDescriptorRange ConstantBuffer1Range;
	ConstantBuffer1Range.NumDescriptors = 3;
	ConstantBuffer1Range.BaseShaderRegister = 0;
	ConstantBuffer1Range.RegisterSpace = 0;
	ConstantBuffer1Range.OffsetInDescriptorsFromTableStart = 0xffffffff;
	ConstantBuffer1Range.RangeType = EFRIRootDescriptorRangeType::CBV;

	FRIRootDescriptorTable ConstantBufferTable;
	ConstantBufferTable.NumRanges = 1;
	ConstantBufferTable.Ranges = &ConstantBuffer1Range;

	FArray<FRIRootParameter> RootParams = {
			FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Vertex, ConstantBufferTable),
			FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Pixel, ConstantBufferTable),
			FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Pixel, Textures),
	};
	FArray<FRIStaticSampler> StaticSamplers = {
			FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 0, 0),
			FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 1, 0),
			FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 2, 0),
			FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 3, 0),
	};


	FRIRasterizerState Rasterizer{ EFRICullMode::Back , EFRIFillMode::Solid };
	FRIDepthStencilState Depth{ EFRIBool::True };
	FRIBlendState Blend{ EFRIBlend::One, EFRIBlend::Zero };

	return Allocator->CreatePipelineStateObject(
		Shaders,
		StaticLitLayout,
		RootParams,
		StaticSamplers,
		RTVFormats,
		Rasterizer,
		Depth,
		Blend
	);

}

FRIPipelineStateObject* CreateQuadPipelineState(FRIDynamicAllocator* Allocator, const ShaderLibraryModule& Shaders, const FArray<EFRITextureFormat>& RTVFormats, uint32 NumPixelSRVs, uint32 NumPixelCBVs)
{
	FRIInputLayout StaticLitLayout = {
		.NumElements = 2,
		.DeclarationElements = new FRIInputElementDesc[2]
		{
			FRIInputElementDesc("POSITION", EFRIAttributeType::Float3, 0, EFRIAttribUsage::PerVertex),
			FRIInputElementDesc("TEXCOORD", EFRIAttributeType::Float2, 1, EFRIAttribUsage::PerVertex),
		}
	};

	// Texture 0

	FRIDescriptorRange Texture0Range;
	Texture0Range.NumDescriptors = NumPixelSRVs;
	Texture0Range.BaseShaderRegister = 0;
	Texture0Range.RegisterSpace = 0;
	Texture0Range.OffsetInDescriptorsFromTableStart = 0xffffffff;
	Texture0Range.RangeType = EFRIRootDescriptorRangeType::SRV;

	FRIRootDescriptorTable Textures;
	Textures.NumRanges = 1;
	Textures.Ranges = &Texture0Range;

	// CBV

	FRIDescriptorRange ConstantBuffer1Range;
	ConstantBuffer1Range.NumDescriptors = NumPixelCBVs;
	ConstantBuffer1Range.BaseShaderRegister = 0;
	ConstantBuffer1Range.RegisterSpace = 0;
	ConstantBuffer1Range.OffsetInDescriptorsFromTableStart = 0xffffffff;
	ConstantBuffer1Range.RangeType = EFRIRootDescriptorRangeType::CBV;

	FRIRootDescriptorTable ConstantBufferTable;
	ConstantBufferTable.NumRanges = 1;
	ConstantBufferTable.Ranges = &ConstantBuffer1Range;

	FArray<FRIStaticSampler> StaticSamplers;
	for (uint32 idx = 0; idx < NumPixelSRVs; idx++)
	{
		StaticSamplers.Add(FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, idx, 0));
	}

	FArray<FRIRootParameter> RootParams = {
			FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Pixel, ConstantBufferTable),
			FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Pixel, Textures),
	};

	FRIRasterizerState Rasterizer{ EFRICullMode::Back , EFRIFillMode::Solid };
	FRIDepthStencilState Depth{ EFRIBool::False };
	FRIBlendState Blend{ EFRIBlend::One, EFRIBlend::Zero };

	return Allocator->CreatePipelineStateObject(
		Shaders,
		StaticLitLayout,
		RootParams,
		StaticSamplers,
		RTVFormats,
		Rasterizer,
		Depth,
		Blend);

}



void DeferredRenderer::CreateResources(FRIContext* renderContext)
{
	
	auto Allocator = renderContext->GetFRIDynamic();
	Viewport = renderContext->GetViewport();
	ShadowmapViewport = FViewportRect(0, 0, ShadowmapResolution, ShadowmapResolution);

	/*DefaultBlend = Allocator->CreateBlendState(EFRIBlend::Src, EFRIBlend::OneMinusSrc, EFRIBlend::Src, EFRIBlend::Dst);
	DisableDepth = Allocator->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);
	DefaultDepth = Allocator->CreateDepthStencilState(EFRIBool::True, EFRIBool::False);
	DefaultRasterizer = Allocator->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);
	SMRasterizer = Allocator->CreateRasterizerState(EFRICullMode::None, EFRIFillMode::Solid);

	CreateRenderUtil(renderContext);

	Shaders.Create(this, renderContext);
	UBuffers.Create(this, renderContext);
	FrameBuffers.Create(this, renderContext);

	Modules.AtmosphereRenderer = new AtmosphereRenderer();
	Modules.ParticleRenderer = new ParticleRenderer();
	Modules.Blur = new BlurRenderer();
	Modules.SMAA = new SMAA();

	FHBAOParameters params;

	params.Radius = 2.0f;
	params.Bias = 0.1f;
	params.NearAO = 2.0f;
	params.FarAO = 1.5f;

	params.BackgroundAOEnable = false;
	params.BackgroundAO_ViewDepth = 10.0f;

	params.ForegroundAOEnable = false;
	params.ForegroundAO_ViewDepth = 1.0f;

	params.PowerExponent = 2.0f;
	params.BlurEnable = true;
	params.BlurSharpness = 16.0f;

	Modules.HBAO = HBAOPlus::Allocate(renderContext, params);

	Modules.AtmosphereRenderer->CreateResources(renderContext);
	Modules.Blur->CreateResources(renderContext);
	Modules.ParticleRenderer->CreateResources(renderContext);
	Modules.SMAA->CreateResources(renderContext);


	FDefaultSamplers::Point			= Allocator->CreateSamplerState(EFRITextureFilter::Point, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat);
	FDefaultSamplers::Bilinear		= Allocator->CreateSamplerState(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat);
	FDefaultSamplers::Trilinear		= Allocator->CreateSamplerState(EFRITextureFilter::Trilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat);
	FDefaultSamplers::Anisotropic4	= Allocator->CreateSamplerState(EFRITextureFilter::Anisotropic4, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat);
	FDefaultSamplers::Anisotropic8	= Allocator->CreateSamplerState(EFRITextureFilter::Anisotropic8, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat);
	*/

	auto CmdContext = renderContext->GetCommandContext(0);
	CmdContext->OpenCommandList();

	FAssetManager Content;
	Content.RenderContext = renderContext;
	Content.Connect("./Assets/");

	/*
	* 
	*	Shaders
	* 
	*/

	auto DeferredShaders = Content.Load<ShaderLibrary>("Shaders/deferred_dx.fslib");
	auto AtmoShaders = Content.Load<ShaderLibrary>("Shaders/atmosphere_dx.fslib");

	//Pipelines.GShader =			Allocator->CreateShaderPipeline(DeferredShaders.Modules["GBuffer"]);
	//Pipelines.GSkinnedShader =	Allocator->CreateShaderPipeline(DeferredShaders.Modules["GBufferSkinned"]);
	//Pipelines.Lighting =		Allocator->CreateShaderPipeline(DeferredShaders.Modules["Lighting"]);
	//Pipelines.PostProcess =		Allocator->CreateShaderPipeline(DeferredShaders.Modules["PostProcess"]);
	//Pipelines.SMShader =		Allocator->CreateShaderPipeline(DeferredShaders.Modules["Shadow"]);
	//Pipelines.SMSkinnedShader = Allocator->CreateShaderPipeline(DeferredShaders.Modules["ShadowSkinned"]);
	//Pipelines.ViewSpaceDecalShader = Allocator->CreateShaderPipeline(DeferredShaders.Modules["ViewSpaceDecal"]);
	//Pipelines.SkySphere = Allocator->CreateShaderPipeline(AtmoShaders.Modules["AtmosphereBasic"]);

	FArray<EFRITextureFormat> GBufferRTVFormats = 
	{ 
		EFRITextureFormat::RGBA8UNORM,
		EFRITextureFormat::RGBA8UNORM,
		EFRITextureFormat::RGBA8UNORM,
		EFRITextureFormat::RGBA8UNORM 
	};
	FArray<EFRITextureFormat> GBufferDepthRTVFormats =
	{
		EFRITextureFormat::RG32F
	};

	Pipelines.GShader			= CreateGeometryPipelineState(Allocator, DeferredShaders.Modules["GBuffer"], GBufferRTVFormats);
	//Pipelines.GSkinnedShader	= CreateGeometrySkinnedPipelineState(Allocator, DeferredShaders.Modules["GBufferSkinned"], GBufferRTVFormats);
	Pipelines.SMShader			= CreateGeometryPipelineState(Allocator, DeferredShaders.Modules["Shadow"], GBufferDepthRTVFormats);
	//Pipelines.SMSkinnedShader	= CreateGeometrySkinnedPipelineState(Allocator, DeferredShaders.Modules["ShadowSkinned"], GBufferDepthRTVFormats);
	Pipelines.Lighting =	CreateQuadPipelineState(Allocator, DeferredShaders.Modules["Lighting"], { EFRITextureFormat::RGBA16F }, 10, 10);
	Pipelines.PostProcess = CreateQuadPipelineState(Allocator, DeferredShaders.Modules["PostProcess"], { EFRITextureFormat::RGBA8UNORM }, 1, 1);

	/*
	* 
	*	Constant Buffers
	* 
	*/

	CBuffers.CameraMatrix =			Allocator->CreateConstantBuffer(sizeof(Camera) * 2, EFRIAccess::Write, EFRIUsage::Dynamic);
	CBuffers.Transform =			Allocator->CreateConstantBuffer(sizeof(FMatrix4) * 2, EFRIAccess::Write, EFRIUsage::Dynamic);
	CBuffers.JointData =			Allocator->CreateConstantBuffer(sizeof(FMatrix4) * 128, EFRIAccess::Write, EFRIUsage::Dynamic);
	CBuffers.DLight =				Allocator->CreateConstantBuffer(DirectionalLight::GetStageMemorySize() * 8, EFRIAccess::Write, EFRIUsage::Dynamic);
	CBuffers.PLight =				Allocator->CreateConstantBuffer(PointLight::GetStageMemorySize() * 8, EFRIAccess::Write, EFRIUsage::Dynamic);
	CBuffers.SLight =				Allocator->CreateConstantBuffer(SpotLight::GetStageMemorySize() * 8, EFRIAccess::Write, EFRIUsage::Dynamic);
	CBuffers.LightingConstants =	Allocator->CreateConstantBuffer(76 * sizeof(float), EFRIAccess::Write, EFRIUsage::Dynamic);

	//Material = Allocator->CreateConstantBuffer(Material::, EFRIAccess::Write, EFRIUsage::Dynamic);
	//CBuffers.CascadeData = Allocator->CreateConstantBuffer(sizeof(FViewFrustumInfo) * SM_CASCADES, EFRIAccess::Write, EFRIUsage::Dynamic);


	/*
	* 
	*	GBuffer
	* 
	*/

	IVector2 viewportSize = renderContext->GetViewport().Size;

	Depth.Surface = Allocator->CreateTexture2D(viewportSize.x, viewportSize.y, 1, EFRIAccess::None, EFRITextureFormat::DEPTH32);
	Depth.DSV = Allocator->CreateDepthStencilView(Depth.Surface);
	Depth.SRV = Allocator->CreateShaderResourceView(Depth.Surface);
	Depth.Surface->Rename("GBuffer::Depth");

	Albedo.Surface = Allocator->CreateTexture2D(viewportSize.x, viewportSize.y, 1, EFRIAccess::None, EFRITextureFormat::RGBA8UNORM);
	Albedo.RTV = Allocator->CreateRenderTargetView(Albedo.Surface);
	Albedo.SRV = Allocator->CreateShaderResourceView(Albedo.Surface);
	Albedo.Surface->Rename("GBuffer::Albedo");

	Normal.Surface = Allocator->CreateTexture2D(viewportSize.x, viewportSize.y, 1, EFRIAccess::None, EFRITextureFormat::RGBA8UNORM);
	Normal.RTV = Allocator->CreateRenderTargetView(Normal.Surface);
	Normal.SRV = Allocator->CreateShaderResourceView(Normal.Surface);
	Normal.Surface->Rename("GBuffer::Normal");

	MetallicRoughness.Surface = Allocator->CreateTexture2D(viewportSize.x, viewportSize.y, 1, EFRIAccess::None, EFRITextureFormat::RGBA8UNORM);
	MetallicRoughness.RTV = Allocator->CreateRenderTargetView(MetallicRoughness.Surface);
	MetallicRoughness.SRV = Allocator->CreateShaderResourceView(MetallicRoughness.Surface);
	MetallicRoughness.Surface->Rename("GBuffer::MetallicRoughness");

	Emissive.Surface = Allocator->CreateTexture2D(viewportSize.x, viewportSize.y, 1, EFRIAccess::None, EFRITextureFormat::RGBA8UNORM);
	Emissive.RTV = Allocator->CreateRenderTargetView(Emissive.Surface);
	Emissive.SRV = Allocator->CreateShaderResourceView(Emissive.Surface);
	Emissive.Surface->Rename("GBuffer::Emissive");

	/*
	* 
	*	Lighting
	* 
	*/

	//Lighting.AOTexture					= Allocator->CreateTexture2D(viewportSize.x, viewportSize.y, 1, EFRIAccess::None, EFRITextureFormat::R8UNORM);
	//Lighting.PostProcessTex				= Allocator->CreateTexture2D(viewportSize.x, viewportSize.y, 1, EFRIAccess::None, EFRITextureFormat::RGBA8UNORM);
	Lighting.Surface					= Allocator->CreateTexture2D(viewportSize.x, viewportSize.y, 1, EFRIAccess::None, EFRITextureFormat::RGBA16F);

	Lighting.RTV = Allocator->CreateRenderTargetView(Lighting.Surface);
	Lighting.SRV = Allocator->CreateShaderResourceView(Lighting.Surface);
	Lighting.Surface->Rename("Lighting::Final");

	//Lighting.LightAndTransluscentTex		= Allocator->CreateTexture2D(viewportSize.x, viewportSize.y, 1, EFRIAccess::None, EFRITextureFormat::RGBA16F);


	/*
	*
	*	Look-up tables
	* 
	*/

	FUniquePtr<FRICreationDescriptor> DataDesc = new FRICreationDescriptor(brdfTex, BRDF_LUT_SIZE);
	BRDF.Surface = Allocator->CreateTexture2D(
		BRDF_LUT_WIDTH,
		BRDF_LUT_HEIGHT,
		1,
		EFRIAccess::None,
		EFRITextureFormat::RG8UNORM,
		DataDesc);

	BRDF.Surface->Rename("LUT::BRDF");
	BRDF.SRV = Allocator->CreateShaderResourceView(BRDF.Surface);

	/*
	* 
	*	Render Targets
	* 
	*/
	/*
	Depth.ShadowmapArray = Allocator->CreateTexture2DArray(ShadowmapResolution, ShadowmapResolution, SM_CASCADES, 1, EFRIAccess::None, EFRITextureFormat::RG32F);
	for (int layer = 0; layer < SM_CASCADES; layer++)
	{
		Depth.ShadowmapViews.Add(Allocator->CreateRenderTargetView(Depth.ShadowmapArray, layer));
	}*/

	FRenderUtil::CreateResources(Allocator);
	CmdContext->CloseCommandList();
}
/*
void DeferredRenderer::RecreateResources(FRIContext* renderContext, FRIContext* prevContext)
{

}*/

/*
void DeferredRenderer::CreateRenderUtil(FRIContext* renderContext)
{
	auto Allocator = renderContext->GetFRIDynamic();

	/* Create Render Stage resources 

	FRenderUtil::VertexBuffer = Allocator->CreateVertexBuffer(
		FRenderUtil::quadVertices.Length(),
		sizeof(FVertex_PositionTexture),
		EFRIAccess::None,
		EFRIUsage::Default, 
		FRICreationDescriptor(
			FRenderUtil::quadVertices.Begin(),
			FRenderUtil::quadVertices.ByteSize()
		));


	FRIVertexShader* scrQuadSignatureShader = NULL;
	if (renderContext->InstanceDescription.RenderFramework == EFRIRendererFramework::DX11)
	{
		scrQuadSignatureShader = Allocator->CreateVertexShader(IOFileStream("Assets/Shaders/signature/dx/bin/CombineQuad.signature.cso").ReadBytes());
	}


	FArray<FRIInputDesc> LayoutDesc;
	LayoutDesc.Add(FRIInputDesc(FRenderUtil::VertexDeclComp, 0));

	FRenderUtil::VertexDeclaration = cmdList.GetDynamic()->CreateInputLayout(LayoutDesc, scrQuadSignatureShader);
	cmdList.GetDynamic()->AttachInputLayout(FRenderUtil::VertexBuffer, FRenderUtil::VertexDeclaration);

	FRenderUtil::IndexBuffer = cmdList.GetDynamic()->CreateIndexBuffer(
		FRenderUtil::quadElementData.ByteSize(),
		EFRIAccess::None,
		EFRIUsage::Default,
		FRICreationDescriptor(
			FRenderUtil::quadElementData.Begin(),
			FRenderUtil::quadElementData.ByteSize()
		)
	);
}*/

/*
*  G-BUFFER Rendering
*  Albedo
*  Normal
*  Detail (Roughness, Metallic, Height, AO)GSkinnedShader
*  Emissive
*/

void DeferredRenderer::RenderGBuffer(FRICommandList& cmdList, const Camera& camera)
{
	cmdList.StageResources(CBuffers.CameraMatrix, 
		[&](FRIMemoryMap& GPUMem)
		{
			camera.StageMemory(GPUMem);
		});

	cmdList.SetViewport(Viewport);
	cmdList.BindRenderTargets({ Normal.RTV, Albedo.RTV, MetallicRoughness.RTV, Emissive.RTV }, Depth.DSV);

	cmdList.ClearRenderTarget(Normal.RTV, Color32::Transparent);
	cmdList.ClearRenderTarget(Albedo.RTV,				Color32::Transparent);
	cmdList.ClearRenderTarget(MetallicRoughness.RTV,	Color32::Transparent);
	cmdList.ClearRenderTarget(Emissive.RTV,				Color32::Transparent);
	cmdList.ClearDepthStencil(Depth.DSV,				1.0f);

	/* Static Geometry */
	cmdList.SetPipelineState(Pipelines.GShader);
	// RenderEnvironmentStatic(cmdList);
	RenderGeometry(cmdList, camera);

	//cmdList.SetPipelineState(Pipelines.GSkinnedShader);
	//RenderGeometryRigged(cmdList);
}

/*
*	Regular Geometry
*/

void DeferredRenderer::RenderGeometry(FRICommandList& cmdList, const Camera& camera)
{
	pScene->System<Model>()->ForEach([&](Entity entity, Model& model)
		{
			if (model.Material.UsesTransluscency == EFRIBool::True)
				return;

			/* Stage DataBuffer */
			cmdList.StageResources(CBuffers.Transform, [&](FRIMemoryMap& stageMem)
				{
					FMatrix4 tmpWorld = entity.GlobalTransform().GetMatrix();

					stageMem.Load(tmpWorld);
					stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

				});

			/* Stage Sampler and Textures */

			for (uint32 layerIdx = 0; layerIdx < (uint32)EMaterialLayer::NUM_LAYERS; layerIdx++)
			{
				auto layerView = model.Material.GetLayer((EMaterialLayer)layerIdx).View;
				auto layerSampler = model.Material.GetLayer((EMaterialLayer)layerIdx).Sampler;
				if (layerView)//&& layerSampler)
				{
					cmdList.SetShaderResourceViewPS(layerIdx, layerView);
					//cmdList.SetShaderSamplerPS(layerIdx, layerSampler);
				}
			}

			/* Stage emission properties 
			cmdList.StageResources(CBuffers.Material, [&](FRIMemoryMap& stageMem)
				{
					model.Material.StageMemory(stageMem);
				});

			/* Draw */
			model.AddToRenderList(cmdList);
		}
	);
	
	pScene->System<RenderObject>()->ForEach([&](Entity entity, RenderObject& object)
		{
			/* Stage DataBuffer*/
			cmdList.StageResources(CBuffers.Transform, [&](FRIMemoryMap& stageMem)
				{
					FMatrix4 tmpWorld = entity.GlobalTransform().GetMatrix();

					stageMem.Load(tmpWorld);
					stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

				});

			/* Draw*/
			object.RenderList->AddToCmdList(cmdList, GRenderMode::Material);
		}

	);
}
/*
void DeferredRenderer::RenderGeometryDepth(FRICommandList& cmdList)
{
	pScene->System<Model>()->ForEach([&](Entity entity, Model& model)
		{
			if (model.Material.UsesTransluscency == EFRIBool::True)
				return;

			if (!model.EnableShadows)
				return;

			/* Stage DataBuffer 
			cmdList.StageResources(CBuffers.Transform, [&](FRIMemoryMap& stageMem)
				{
					FMatrix4 tmpWorld = entity.GlobalTransform().GetMatrix();

					stageMem.Load(tmpWorld);
					stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

				});

			model.AddToRenderList(cmdList);
		}
	);
}*/

/*
*	Rigged Geometry
*/
/*
void DeferredRenderer::RenderGeometryRigged(FRICommandList& cmdList)
{
	pScene->System<RiggedModel>()->ForEach([&](Entity entity, RiggedModel& skinnedModel)
		{
			if (skinnedModel.Material.UsesTransluscency == EFRIBool::True)
				return;

			/* Stage DataBuffer 
			cmdList.StageResources(CBuffers.Transform, [&](FRIMemoryMap& stageMem)
				{
					FMatrix4 tmpWorld = entity.GlobalTransform().GetMatrix();

					stageMem.Load(tmpWorld);
					stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

				});
			cmdList.StageResources(CBuffers.JointData, [&](FRIMemoryMap& stageMem)
				{
					const FArray<FMatrix4>& jointMatrices = skinnedModel.Mesh.MeshSkeleton.GetJointTransforms();
					stageMem.Load(jointMatrices.Begin(), jointMatrices.ByteSize());

				});

			/* Stage Samplers 

			for (uint32 layerIdx = 0; layerIdx < (uint32)EMaterialLayer::NUM_LAYERS; layerIdx++)
			{
				auto layerView = skinnedModel.Material.GetLayer((EMaterialLayer)layerIdx).View;
				auto layerSampler = skinnedModel.Material.GetLayer((EMaterialLayer)layerIdx).Sampler;

				if (layerView)
				{
					cmdList.SetShaderResourceViewPS(layerIdx, layerView);
					//cmdList.SetShaderSamplerState(map, sampler);
				}
			}

			/* Stage emission properties 
			cmdList.StageResources(CBuffers.Material, [&](FRIMemoryMap& stageMem)
				{
					skinnedModel.Material.StageMemory(stageMem);
				});

			/* Draw 
			skinnedModel.AddToRenderList(cmdList);
		}
	);
}*/
/*
void DeferredRenderer::RenderGeometryRiggedDepth(FRICommandList& cmdList)
{
	pScene->System<RiggedModel>()->ForEach([&](Entity entity, RiggedModel& skinnedModel)
		{
			if (skinnedModel.Material.UsesTransluscency == EFRIBool::True)
				return;

			/* Stage DataBuffer 
			cmdList.StageResources(CBuffers.Transform, [&](FRIMemoryMap& stageMem)
				{
					FMatrix4 tmpWorld = entity.GlobalTransform().GetMatrix();

					stageMem.Load(tmpWorld);
					stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

				});
			cmdList.StageResources(CBuffers.JointData, [&](FRIMemoryMap& stageMem)
				{
					const FArray<FMatrix4>& jointMatrices = skinnedModel.Mesh.MeshSkeleton.GetJointTransforms();
					stageMem.Load(jointMatrices.Begin(), jointMatrices.ByteSize());

				});

			/* Draw 
			skinnedModel.AddToRenderList(cmdList);
		}
	);
}*/
/*
void DeferredRenderer::RenderEnvironmentDynamic(FRICommandList& cmdList)
{

}*/
/*
void DeferredRenderer::RenderDLightShadowmaps(FRICommandList& cmdList, const DirectionalLight& Light, uint32 numRenderTargets, FRIRenderTargetView** Target)
{
	cmdList.SetViewport(ShadowmapViewport);
	{
		cmdList.StageResources(CBuffers.Transform, [&](FRIMemoryMap& memory)
			{
				memory.Load(FMatrix4::Identity());
				memory.Load(FMatrix4::Identity());

			});

		for (int cascade = 0; cascade < SM_CASCADES; cascade++)
		{
			cmdList.BindRenderTargets({ Target[cascade] }, NULL);
			cmdList.ClearRenderTarget(Target[cascade], Color32::White);

			// Stage cascade camera buffer
			cmdList.StageResources(CBuffers.CameraMatrix, [&](FRIMemoryMap& memory)
				{
					memory.Load(&Light.FrustumData[cascade].View, sizeof(Camera));
				});


			/*  Static Shadowed Scene 
			cmdList.SetPipelineState(Pipelines.SMShader);

			RenderEnvironmentStaticDepth(cmdList);
			RenderGeometryDepth(cmdList);

			/*  Skinned Shadowed Scene 
			//cmdList.SetPipelineState(Pipelines.SMSkinnedShader);
			//RenderGeometryRiggedDepth(cmdList);
		}
	}


	//cmdList.GetDynamic()->FlushMipMaps(FrameBuffers.ShadowmapArray);
}
void DeferredRenderer::RenderSLightShadowmaps(FRICommandList& cmdList, const SpotLight& Light, uint32 numRenderTargets, FRIRenderTargetView** Target)
{

}

void DeferredRenderer::RenderPLightShadowmaps(FRICommandList& cmdList, const PointLight& Light, uint32 numRenderTargets, FRIRenderTargetView** Target)
{

}
*/
void DeferredRenderer::BeginRender(FRICommandList& cmdList)
{
	/*
	cmdList.SetShaderConstantBuffer(UBO_SLOT::CAMERA,	UBuffers.CameraMatrix.GPU, EFRI_Vertex | EFRI_Pixel);
	cmdList.SetShaderConstantBuffer(UBO_SLOT::TRANSFORM,	UBuffers.Transform.GPU, EFRI_Vertex | EFRI_Pixel);
	cmdList.SetShaderConstantBuffer(UBO_SLOT::JOINTS,	UBuffers.JointData.GPU, EFRI_Vertex);
	cmdList.SetShaderConstantBuffer(UBO_SLOT::MATERIAL,	UBuffers.Material.GPU, EFRI_Vertex | EFRI_Pixel);
	cmdList.SetShaderConstantBuffer(UBO_SLOT::CASCADE,	UBuffers.CascadeData.GPU, EFRI_Vertex | EFRI_Pixel);

	cmdList.SetShaderConstantBuffer(UBO_SLOT::LIGHTING, UBuffers.LightingConstants.GPU, EFRI_Vertex | EFRI_Pixel);

	cmdList.SetShaderConstantBuffer(UBO_SLOT::DLIGHT_DATA, UBuffers.DLight.GPU, EFRI_Pixel);
	cmdList.SetShaderConstantBuffer(UBO_SLOT::PLIGHT_DATA, UBuffers.PLight.GPU, EFRI_Pixel);
	cmdList.SetShaderConstantBuffer(UBO_SLOT::SLIGHT_DATA, UBuffers.SLight.GPU, EFRI_Pixel);

	for (int i = 0; i < 12; i++)
	{
		cmdList.SetShaderSamplerState(i, FDefaultSamplers::Bilinear);
	}
	*/

	cmdList.SetConstantBufferViewVS(0, CBuffers.CameraMatrix->GetView());
	cmdList.SetConstantBufferViewVS(1, CBuffers.Transform->GetView());
	//
	cmdList.SetConstantBufferViewPS(0, CBuffers.CameraMatrix->GetView());
	cmdList.SetConstantBufferViewPS(1, CBuffers.Transform->GetView());

}

void DeferredRenderer::Render(FRICommandList& cmdList)
{

	if (!pScene->Elements.Contains("GameCamera") || !pScene->Elements["GameCamera"].HasComponent<Camera>())
		return;
	if (!pScene->Elements.Contains("Sun") || !pScene->Elements["Sun"].HasComponent<DirectionalLight>())
		return;

	//if (!pScene->Elements.Contains("Environment") || !pScene->Elements["Environment"].HasComponent<EnvironmentMap>())
		//return;

	Camera& sceneCamera = pScene->Elements["GameCamera"].Component<Camera>();
	FTransform& cameraPos = pScene->Elements["GameCamera"].Component<FTransform>();
	DirectionalLight& sun = pScene->Elements["Sun"].Component<DirectionalLight>();
	//EnvironmentMap& env = pScene->Elements["Environment"].Component<EnvironmentMap>();


	StageLightData(cmdList, sceneCamera, sun);
	//RenderShadowmaps(cmdList, sun);
	RenderGBuffer(cmdList, sceneCamera);

	//RenderGI(cmdList, camera);
	RenderLighting(cmdList, sceneCamera);

	//RenderTransluscency(cmdList);
	RenderPostProcess(cmdList);
}

DeferredRenderer::~DeferredRenderer()
{

}

void DeferredRenderer::EndRender(FRICommandList& cmdList)
{


}
void DeferredRenderer::AttachToScene(Scene* scene)
{
	pScene = scene;
}

/*
void DeferredRenderer::RenderGI(FRICommandList& cmdList, const Camera& Camera)
{
	//Modules.HBAO->RenderAO(FrameBuffers.GBuffer->GetDepthBuffer(), FrameBuffers.Normal, FrameBuffers.AOBuffer, Camera.Projection, Camera.View);
}*/

/*
void DeferredRenderer::RenderEnvironmentStatic(FRICommandList& cmdList)
{
	
	cmdList.StageResources(CBuffers.Transform, [&](FRIMemoryMap& stageMem)
		{
			stageMem.Load(FMatrix4::Identity());
			stageMem.Load(FMatrix4::Identity());
		});

	pScene->System<Level>()->ForEach([&](Entity& entity, Level& level)
		{

			for (auto& geom : level.LevelGeometry.Leafs)
			{

				/* Stage Sampler and Textures 

				for (uint32 layerIdx = 0; layerIdx < (uint32)EMaterialLayer::NUM_LAYERS; layerIdx++)
				{
					auto layerView = geom->Material.GetLayer((EMaterialLayer)layerIdx).View;
					auto layerSampler = geom->Material.GetLayer((EMaterialLayer)layerIdx).Sampler;
					if (layerView)//&& layerSampler)
					{
						cmdList.SetShaderResourceViewPS(layerIdx, layerView);
						//cmdList.SetShaderSamplerPS(layerIdx, layerSampler);
					}
				}

				cmdList.SetGeometrySource(geom->VertexBuffer, geom->IndexBuffer);
				cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, geom->IndexBuffer->IndexCount, EFRIIndexType::UInt32);
			}
		});
}*/
/*
void DeferredRenderer::RenderDecals(FRICommandList& cmdList)
{
	/*
	cmdList.SetShaderResource(4, FrameBuffers.GBuffer->GetDepthBuffer()->View());

	scene->System<ViewSpaceDecal>()->ForEach([&](Entity entity, ViewSpaceDecal& decal)
		{

			cmdList.StageResourcesLambda(UBuffers.Transform, [&](FRIMemoryMap& stageMem)
				{
					FMatrix4 tmpWorld = entity.GlobalTransform().GetMatrix();

					stageMem.Load(tmpWorld);
					stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

				});

			auto sampler = decal.material.Sampler;
			for (int map = 0; map < (uint32)EMaterialMap::MAX_MAPS; map++)
			{
				FRITexture2D* textureMap = decal.material.GetMap((EMaterialMap)map);
				if (textureMap)
				{
					cmdList.SetShaderResource(map, textureMap->View());
					cmdList.SetShaderSamplerState(map, sampler);
				}
			}

			decal.AddToRenderList(cmdList);
		});
}*/


/*
void DeferredRenderer::RenderSceneBackground(FRICommandList& cmdList, const Camera& camera)
{
	
/*	if (scene->SceneBackground == SceneBackgroundMode::None)
		return;

	if (scene->SceneBackground == SceneBackgroundMode::SolidColor)
		return;

	Entity envMapEntity;

	if (scene->EnvironmentMapSelection == EnvironmentMapSelectionMode::First)
	{
		envMapEntity = scene->System<EnvironmentMap>()->First();
	}
	else
	{
		float MinDistance = 9999999.0f;

		scene->System<EnvironmentMap>()->ForEach([&](Entity entity, EnvironmentMap& envMap)
			{
				float distance = (entity.GlobalTransform().Position - scene->Elements["Camera"].GlobalTransform().Position).Length();

				if (distance < MinDistance)
				{
					MinDistance = distance;
					envMapEntity = entity;
				}

			});

	}

	if (envMapEntity.IsValid())
	{
		cmdList.SetShaderResource(10, envMapEntity.Component<EnvironmentMap>().Skymap->View());
		cmdList.SetShaderPipeline(Shaders.SkySphere);
		FRenderUtil::DrawScreenQuad(cmdList);
	}
}*/


void DeferredRenderer::RenderLighting(
	FRICommandList& cmdList, 
	const Camera& Camera
)
{
	//auto emissiveTexBlurred = Modules.Blur->Render(cmdList, FrameBuffers.Emissive);
	//auto emissiveTexBlurred = FrameBuffers.Emissive;

	cmdList.SetPipelineState(Pipelines.Lighting);

	cmdList.SetShaderResourceViewPS(0,  Depth.SRV);
	cmdList.SetShaderResourceViewPS(1,	Normal.SRV);
	cmdList.SetShaderResourceViewPS(2,	Albedo.SRV);
	cmdList.SetShaderResourceViewPS(3,	MetallicRoughness.SRV);
	cmdList.SetShaderResourceViewPS(4,	Emissive.SRV);
	//cmdList.SetShaderResourceViewPS(LI_SLOT::AO,		GBuffer.SRV);
	cmdList.SetShaderResourceViewPS(6, Depth.SRV);

	cmdList.SetShaderResourceViewPS(7,			BRDF.SRV);
	//cmdList.SetShaderResourceViewPS(LI_SLOT::SPECULAR,		env.Specular->View());
	//cmdList.SetShaderResourceViewPS(LI_SLOT::IRRADIANCE,	env.Irradiance->View());

	cmdList.SetConstantBufferViewPS(0, CBuffers.CameraMatrix->GetView());
	cmdList.SetConstantBufferViewPS(4, CBuffers.LightingConstants->GetView());
	cmdList.SetConstantBufferViewPS(6, CBuffers.DLight->GetView());
	cmdList.SetConstantBufferViewPS(7, CBuffers.PLight->GetView());
	cmdList.SetConstantBufferViewPS(8, CBuffers.SLight->GetView());

	cmdList.BindRenderTargets({ Lighting.RTV }, NULL);
	cmdList.ClearRenderTarget({ Lighting.RTV }, pScene->BackgroundColor);
	//cmdList.ClearDepthStencil(NULL, 1.0f);
	{
		/*cmdList.SetBlendState(DefaultBlend);
		cmdList.SetRasterizerState(DefaultRasterizer);
		cmdList.SetDepthStencilState(DisableDepth);*/

		//RenderSceneBackground(cmdList, Camera);


		FRenderUtil::DrawScreenQuad(cmdList);

	}
}
/*void DeferredRenderer::RenderTransluscency(FRICommandList& cmdList)
{
	// Copy Lighting and depth data from Lighting stage onto FrameBuffer
	/*cmdList.GetDynamic()->CopyResource(FrameBuffers.LightingTex, FrameBuffers.LightAndTransluscentTex);
	cmdList.GetDynamic()->CopyResource(FrameBuffers.GBuffer->GetDepthBuffer(), FrameBuffers.TransluscentBuffer->GetDepthBuffer());

	// Set Albedo Normal And Depth to be available for Forward rendering reads (i.e transluscency, refraction, SSR and other screen space effects)
	cmdList.SetShaderResource(LI_SLOT::DEPTH, FrameBuffers.GBuffer->GetDepthBuffer()->View());
	cmdList.SetShaderResource(LI_SLOT::NORMAL, FrameBuffers.Normal->View());
	cmdList.SetShaderResource(LI_SLOT::ALBEDO, FrameBuffers.LightingTex->View());

	cmdList.BindFrameBuffer(FrameBuffers.TransluscentBuffer);
	{
		// Transluscent Instructions
		cmdList.SetBlendState(DefaultBlend);
		cmdList.SetRasterizerState(DefaultRasterizer);
		cmdList.SetDepthStencilState(DefaultDepth);

		scene->System<RenderObject, FTransform>()->ForEach([&](Entity ent, RenderObject& object, FTransform& transformComponent)
			{
				/* Stage DataBuffer
				cmdList.StageResourcesLambda(UBuffers.Transform, [&](FRIMemoryMap& stageMem)
					{
						FMatrix4 tmpWorld = transformComponent.GetMatrix();

						stageMem.Load(tmpWorld);
						stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

					});

				/* Draw
				object.RenderList->AddToCmdList(cmdList, GRenderMode::Transluscent);
			}

		);

		scene->System<ParticleManager>()->ForEach([&](Entity ent, ParticleManager& particleManager)
			{
				Modules.ParticleRenderer->RenderSystem(cmdList, particleManager.ParticleSystemPtr);
			});

	}
}*/

void DeferredRenderer::RenderPostProcess(FRICommandList& cmdList)
{
	/*
	Modules.SMAA->SubmitPass(cmdList, FrameBuffers.LightAndTransluscentTex);
	auto PPInput = Modules.SMAA->GetOutput();


	cmdList.SetRasterizerState(DefaultRasterizer);
	cmdList.SetBlendState(DefaultBlend);
	cmdList.SetDepthStencilState(DisableDepth);*/

	cmdList.SetPipelineState(Pipelines.PostProcess);
	cmdList.SetShaderResourceViewPS(0, Lighting.SRV);

	cmdList.BindRenderTargets({}, NULL);
	cmdList.ClearRenderTarget(NULL, pScene->BackgroundColor);
	cmdList.ClearDepthStencil(NULL, 1.0f);
	{
		FRenderUtil::DrawScreenQuad(cmdList);
	}
}
/*
void DeferredRenderer::Present(FRICommandList& cmdList)
{

}*/




void DeferredRenderer::StageLightData(FRICommandList& cmdList, const Camera& camera, const DirectionalLight Sun)
{
	
	auto DLightSystem = pScene->System<DirectionalLight>();
	auto PLightSystem = pScene->System<PointLight>();
	auto SLightSystem = pScene->System<SpotLight>();

	uint32 DLightNum = DLightSystem->Count();
	uint32 PLightNum = PLightSystem->Count();
	uint32 SLightNum = SLightSystem->Count();

	// Lighting Pass Constants

	FMatrix4 inverseView = FMatrix4::Inverse(camera.View);

	cmdList.StageResources(CBuffers.LightingConstants, [=](FRIMemoryMap& GPUMemory)
		{
			GPUMemory
				<< inverseView[3]
				<< camera.NearPlane()
				<< camera.FarPlane()
				<< (float)DLightNum
				<< (float)PLightNum
				<< (float)SLightNum;

		});

	// Cascade Data

	/*cmdList.StageResources(CBuffers.CascadeData, [=](FRIMemoryMap& GPUMemory)
		{
			for (int i = 0; i < SM_CASCADES; i++)
			{
				//FMatrix4 toLight = SunRef.FrustumInfo[i].View * SunRef.FrustumInfo[i].Projection;

				GPUMemory << Sun.FrustumData[i].View;
				GPUMemory << Sun.FrustumData[i].Projection;
				GPUMemory << FVector4(Sun.FrustumData[i].Depth);
			}
		});*/


	// Lights Data

	cmdList.StageResources(CBuffers.DLight, [=](FRIMemoryMap& GPUMemory)
		{
			pScene->System<DirectionalLight>()->ForEach([&](Entity ent, DirectionalLight& light)
				{
					light.StageMemory(GPUMemory, camera.View);
				});

		});

	cmdList.StageResources(CBuffers.PLight, [=](FRIMemoryMap& GPUMemory)
		{
			pScene->System<PointLight>()->ForEach([&](Entity ent, PointLight& light)
				{
					light.StageMemory(GPUMemory, camera.View);
				});

		});

	cmdList.StageResources(CBuffers.SLight, [&](FRIMemoryMap& GPUMemory)
		{
			pScene->System<SpotLight>()->ForEach([&](Entity ent, SpotLight& light)
				{
					light.StageMemory(GPUMemory, camera.View);
				});

		});
}
/*
void DeferredRenderer::RenderEnvironmentStaticDepth(FRICommandList& cmdList)
{

}

void DeferredRenderer::RenderEnvironmentDynamicDepth(FRICommandList& cmdList)
{

}
*/
