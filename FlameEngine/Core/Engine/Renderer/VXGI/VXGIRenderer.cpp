#include "VXGIRenderer.h"


#include "Core/Framework/IO/FileStream.h"
#include "../../ContentSystem/Client/AssetImportScripts/Material.h"
#include "Buffers.h"

#include "../DeferredRenderer.h"



#define UBO_SLOT_VOXELIZE 9
#define UBO_SLOT_VXGI 10


struct FTransformBufferStruct
{
	FMatrix4 World;
	FMatrix4 WorldInverseTranspose;
};

struct FMaterialBufferStruct
{
	FVector4 Emissive;
	uint32 HasNormalMap;
	uint32 HasPOMEnabled;
	uint32 HasAOMap;
	uint32 HasEmissive;
	uint32 HasTransluscent;
};

struct FCameraBufferStruct
{
	FViewMatrix View;
	FProjectionMatrix Projection;
	FMatrix4 InverseView;
	FVector4 CameraPosition;
};



void VXGIRenderer::CreateResources(FRIContext* renderContext)
{

	FRICommandList cmdList(renderContext->GetFRIDynamic());


	Shaders = FLocalContent::LoadFromLocal<ShaderLibrary>("shaders/vxgi.fslib", renderContext);

	IVector2 viewportSize = renderContext->GetViewport().Size;

	/* Create Render Stage resources */


	Voxelize = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["VoxelizationGen"]);
	LightingCompute = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["LightCompute"]);
	PropagationCompute = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["LightPropagation"]);
	FlushCompute = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["VoxelFlush"]);
	AnisoCompute = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["AnisotropicCreate"]);

	VXGIBuffer = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FVXGIBuffer)));
	VoxelizeBuffer = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FMatrix4) * 3));
	PropagationData = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(float)*4));


	VoxelAlbedo = cmdList.GetDynamic()->CreateTexture3D(256, 256, 256, EFRITextureFormat::R32UI);
	VoxelNormal = cmdList.GetDynamic()->CreateTexture3D(256, 256, 256, EFRITextureFormat::R32UI);
	VoxelEmission = cmdList.GetDynamic()->CreateTexture3D(256, 256, 256, EFRITextureFormat::R32UI);
	VoxelBaseRadiance = cmdList.GetDynamic()->CreateTexture3D(256, 256, 256, EFRITextureFormat::RGBA8UNORM);
	VoxelFinal = cmdList.GetDynamic()->CreateTexture3D(256, 256, 256, EFRITextureFormat::RGBA8UNORM);

	for (int i = 0; i < 6; i++)
	{
		VoxelAnisotropic[i] = cmdList.GetDynamic()->CreateTexture3D(128, 128, 128, EFRITextureFormat::RGBA8UNORM);
	}

	OutputTex = cmdList.GetDynamic()->CreateTexture2D(1024, 1024, 0, EFRITextureFormat::RGBA8UNORM);
	OutputFBO = cmdList.GetDynamic()->CreateFrameBuffer({ FRIFrameBufferAttachment(OutputTex) }, false);

	BlendState = cmdList.GetDynamic()->CreateBlendState(EFRIAlphaBlend::Src, EFRIAlphaBlend::OneMinusSrc);
	DepthStencilState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);
	RasterizerState = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::None, EFRIFillMode::Solid);

	FinalDepth = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::True, EFRIBool::False);

	VoxelizeViewport = FViewportRect(0, 0, 1024, 1024);
	OriginalViewport = renderContext->GetViewport();

}

void VXGIRenderer::RecreateResources(FRIContext* renderContext, FRIContext* prevContext)
{

}




void VXGIRenderer::Render(FRICommandList& cmdList, DeferredRenderer* renderer)
{
	cmdList.SetShaderUniformBuffer(UBO_SLOT_VOXELIZE, VoxelizeBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_VXGI, VXGIBuffer);
	cmdList.SetShaderUniformBuffer(11, PropagationData);


	if (NeedsUpdate)
	{
		VoxelizeScene(cmdList, renderer);
		NeedsUpdate = false;
	}

}

void VXGIRenderer::VoxelizeScene(FRICommandList& cmdList, DeferredRenderer* renderer)
{

	FlushVolumes(cmdList);

	AABB volume = scene->GetAABB();
	float sceneDimension = fmaxf(fmaxf(volume.LengthX(), volume.LengthY()), volume.LengthZ());
	float halfDim = sceneDimension / 2.0f;

	FVXGIBuffer buffer;
	buffer.maxTracingDistanceGlobal = 1.0f;
	buffer.bounceStrength = 1.0;
	buffer.aoFalloff = 725.0;
	buffer.aoAlpha = 0.01f;
	buffer.samplingFactor = 0.5f;
	buffer.coneShadowTolerance = 1.0f;
	buffer.coneShadowAperture = 0.03f;

	buffer.voxelVolumeDimension = 256.0f;
	buffer.sceneDimension = sceneDimension;
	buffer.voxelScale = 1.0f / buffer.sceneDimension;
	buffer.voxelSize = buffer.sceneDimension / buffer.voxelVolumeDimension;
	buffer.WorldMin = FVector4(volume.minPoint, 1);
	buffer.WorldMax = FVector4(volume.maxPoint, 1);

	cmdList.StageResourcesLambda(VXGIBuffer, [&](FRIMemoryMap& stageMemory)
		{
			stageMemory.Load(buffer);
		});


	CameraComponent voxelCam(
		FMatrix4::Identity(),
		FOrthographicMatrix(-halfDim, halfDim, halfDim, -halfDim, 0, sceneDimension));


	cmdList.SetViewport(VoxelizeViewport);

	cmdList.StageResourcesLambda(VoxelizeBuffer, [&](FRIMemoryMap& stageMem)
		{
			stageMem.Load(voxelCam.Projection);
			stageMem.Load(voxelCam.Projection);
			stageMem.Load(voxelCam.Projection);
		});



	// Present
	cmdList.GetDynamic()->ClearBufferAndSetUAVs(OutputFBO, Color::Black, new FRITexture3D * [] {VoxelAlbedo, VoxelNormal, VoxelEmission}, 3);
	{

		cmdList.SetShaderPipeline(Voxelize);

		renderer->RenderEnvironmentStatic(cmdList);
		renderer->RenderGeometry(cmdList);
	}

	cmdList.UnbindFrameBuffer();



	InjectRadiance(cmdList);
	GenerateMipMaps(cmdList, VoxelBaseRadiance);


	auto preBounce = VoxelBaseRadiance;
	auto postBounce = VoxelFinal;

	for (uint32 i = 0; i < LightBounces; i++)
	{
		InjectLightBounce(cmdList, preBounce, postBounce, i+1);
		GenerateMipMaps(cmdList, postBounce);

		auto temp = preBounce;
		preBounce = postBounce;
		postBounce = temp;

	}

	VoxelBaseRadiance = postBounce;
	VoxelFinal = preBounce;


	//cmdList.GetDynamic()->FlushMipMaps(voxelRadiance);
	//cmdList.GetDynamic()->SetDepthStencilState(FinalDepth);


	cmdList.SetViewport(OriginalViewport);

}

void VXGIRenderer::FlushVolumes(FRICommandList& cmdList)
{
	cmdList.SetBlendState(BlendState);
	cmdList.SetRasterizerState(RasterizerState);
	cmdList.GetDynamic()->SetDepthStencilState(DepthStencilState);

	cmdList.GetDynamic()->SetUAV(0, VoxelAlbedo);
	cmdList.GetDynamic()->SetUAV(1, VoxelNormal);
	cmdList.GetDynamic()->SetUAV(2, VoxelEmission);
	cmdList.GetDynamic()->SetUAV(3, VoxelBaseRadiance);
	cmdList.GetDynamic()->SetUAV(4, VoxelFinal);

	cmdList.SetShaderPipeline(FlushCompute);
	{
		cmdList.GetDynamic()->DispatchCompute(32, 32, 32);
	}

	cmdList.GetDynamic()->SetUAV(0, NULL);
	cmdList.GetDynamic()->SetUAV(1, NULL);
	cmdList.GetDynamic()->SetUAV(2, NULL);
	cmdList.GetDynamic()->SetUAV(3, NULL);
	cmdList.GetDynamic()->SetUAV(4, NULL);

}

void VXGIRenderer::GenerateMipMaps(FRICommandList& cmdList, FRITexture3D* voxelmap)
{
	// Mipmap Base

	cmdList.SetShaderPipeline(AnisoCompute);
	{

		cmdList.SetShaderSampler(FUniformSampler(0, voxelmap));
		for (int i = 0; i < 6; i++)
		{
			cmdList.GetDynamic()->SetUAV(i, VoxelAnisotropic[i]);
		}

		cmdList.GetDynamic()->DispatchCompute(16, 16, 16);

		for (int i = 0; i < 6; i++)
		{
			cmdList.GetDynamic()->SetUAV(i, NULL);
			cmdList.GetDynamic()->FlushMipMaps(VoxelAnisotropic[i]);
		}

		cmdList.SetShaderSampler(FUniformSampler(0));
	}
}

void VXGIRenderer::InjectRadiance(FRICommandList& cmdList)
{

	cmdList.GetDynamic()->SetUAV(0, VoxelAlbedo);
	cmdList.GetDynamic()->SetUAV(1, VoxelNormal);
	cmdList.GetDynamic()->SetUAV(2, VoxelEmission);
	cmdList.GetDynamic()->SetUAV(3, VoxelBaseRadiance);

	cmdList.SetShaderPipeline(LightingCompute);
	{
		cmdList.GetDynamic()->DispatchCompute(32, 32, 32);
	}

	cmdList.GetDynamic()->SetUAV(0, NULL);
	cmdList.GetDynamic()->SetUAV(1, NULL);
	cmdList.GetDynamic()->SetUAV(2, NULL);
	cmdList.GetDynamic()->SetUAV(3, NULL);
}

void VXGIRenderer::InjectLightBounce(FRICommandList& cmdList, FRITexture3D* base, FRITexture3D* output, uint32 bounceNumber)
{
	cmdList.GetDynamic()->SetUAV(0, VoxelAlbedo);
	cmdList.GetDynamic()->SetUAV(1, VoxelNormal);
	cmdList.GetDynamic()->SetUAV(2, base);
	cmdList.GetDynamic()->SetUAV(3, output);


	cmdList.StageResourcesLambda(PropagationData, [&](FRIMemoryMap& memory) 
		{
			memory.Load((float)bounceNumber);
		});

	for (int i = 0; i < 6; i++)
	{
		cmdList.SetShaderSampler(FUniformSampler(i, VoxelAnisotropic[i]));
	}

	cmdList.SetShaderPipeline(PropagationCompute);
	{
		cmdList.GetDynamic()->DispatchCompute(32, 32, 32);
	}


	for (int i = 0; i < 6; i++)
	{
		cmdList.SetShaderSampler(FUniformSampler(i));
	}

	cmdList.GetDynamic()->SetUAV(0, NULL);
	cmdList.GetDynamic()->SetUAV(1, NULL);
	cmdList.GetDynamic()->SetUAV(2, NULL);
	cmdList.GetDynamic()->SetUAV(3, NULL);
}