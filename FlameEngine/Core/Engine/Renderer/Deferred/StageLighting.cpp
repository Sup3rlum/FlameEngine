#include "RenderStages.h"

#include "../AtmosphereRenderer.h"
#include "../VXGI/Buffers.h"
#include "BRDF.h"
#include "../RenderSystems.h"

struct FCombineBufferStruct : FRIArrayInterface
{
	FMatrix4 InverseView;
	FMatrix4 InverseProjection;
	FVector4 Direction;
	FVector4 Info;
	FVector4 CameraPosition;
	float AmbienceFactor;
	float Exposure;
	float Padding0;
	float Padding1;
};


void DRStageLighting::CreateResources(ShaderLibrary& Shaders, FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());
	IVector2 viewportSize = renderContext->GetViewport().Size;

	LitTexture = cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA32F);

	BRDFLut = cmdList.GetDynamic()->CreateTexture2D(
		BRDF_LUT_WIDTH,
		BRDF_LUT_HEIGHT,
		0,
		EFRITextureFormat::RG8UNORM,
		FRIColorDataFormat(EFRIChannels::RG, EFRIPixelStorage::UnsignedByte),
		FRICreationDescriptor(brdfTex, BRDF_LUT_SIZE));


	FrameBuffer = cmdList.GetDynamic()->CreateFrameBuffer({ FRIFrameBufferAttachment(LitTexture) }, false);
	LightingShader = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["Lighting"]);
	TransluscencyCombineShader = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["TransluscentCombine"]);

	BlendState = cmdList.GetDynamic()->CreateBlendState(EFRIAlphaBlend::Src, EFRIAlphaBlend::OneMinusSrc);
	RasterizerState = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);
	DepthStencilState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::True, EFRIBool::False);


	CascadeCameraBuffer		= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FViewFrustumInfo) * SM_CASCADES));
	DeferredConstantBuffer	= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL,	28 * sizeof(float)));


	DirectionalLightBuffer	= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, DirectionalLight::GetStageMemorySize() * 32));
	PointLightBuffer		= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, PointLight::GetStageMemorySize() * 32));
	SpotLightBuffer			= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, SpotLight::GetStageMemorySize() * 32));

	VXGIBuffer		= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FVXGIBuffer)));



	Atmosphere = new AtmosphereRenderer();
	Atmosphere->CreateResources(renderContext);

}



void DRStageLighting::RecreateResources(ShaderLibrary& Shaders, FRIContext* renderContext, FRIContext* prevContext)
{

}

void DRStageLighting::Prepare(FRICommandList& cmdList, RStageInterface& input)
{


	cmdList.SetBlendState(BlendState);
	cmdList.SetRasterizerState(RasterizerState);
	cmdList.GetDynamic()->SetDepthStencilState(DepthStencilState);

	cmdList.SetShaderUniformBuffer(UBO_SLOT_COMBINE, DeferredConstantBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_CASCADE_CAMERA, CascadeCameraBuffer);

	cmdList.SetShaderUniformBuffer(UBO_SLOT_DLIGHT_DATA, DirectionalLightBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_PLIGHT_DATA, PointLightBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_SLIGHT_DATA, SpotLightBuffer);
	
	cmdList.SetShaderUniformBuffer(UBO_SLOT_VXGI, VXGIBuffer);

	cmdList.SetShaderSampler(FUniformSampler(0, input.GetResource<FRITexture2D>(RS_SLOT_DEPTH)));
	cmdList.SetShaderSampler(FUniformSampler(1, input.GetResource<FRITexture2D>(RS_SLOT_NORMAL)));
	cmdList.SetShaderSampler(FUniformSampler(2, input.GetResource<FRITexture2D>(RS_SLOT_ALBEDO)));
	cmdList.SetShaderSampler(FUniformSampler(3, input.GetResource<FRITexture2D>(RS_SLOT_METALLIC_ROUGHNESS)));
	cmdList.SetShaderSampler(FUniformSampler(5, input.GetResource<FRITexture2D>(RS_SLOT_AO)));
	cmdList.SetShaderSampler(FUniformSampler(6, input.GetResource<FRITexture2D>(RS_SLOT_EMISSIVE)));
	cmdList.SetShaderSampler(FUniformSampler(4, input.GetResource<FRITexture2DArray>(RS_SLOT_SHADOWMAP)));
	cmdList.SetShaderSampler(FUniformSampler(7, BRDFLut));
	cmdList.SetShaderSampler(FUniformSampler(8, input.GetResource<FRITexture2D>(RS_SLOT_TRANSLUSCENCY)));
	cmdList.SetShaderSampler(FUniformSampler(9, input.GetResource<FRITexture3D>(RS_SLOT_RADIANCE)));


	for (int i = 0; i < 6; i++)
	{
		cmdList.SetShaderSampler(FUniformSampler(10+i, input.GetResource<FRITexture3D>(RS_SLOT_VOXEL_ANISO + i)));
	}
}
void DRStageLighting::SubmitPass(FRICommandList& cmdList, Scene* scene)
{
	auto& CameraRef = scene->Camera.Component<CameraComponent>();
	auto& CameraPosRef = scene->Camera.Component<FTransform>();
	auto& SunRef = scene->Sun.Component<DirectionalLight>();

	AABB volume = scene->GetAABB();
	float sceneDimension = fmaxf(fmaxf(volume.LengthX(), volume.LengthY()), volume.LengthZ());

	FVXGIBuffer buffer;
	buffer.maxTracingDistanceGlobal = 1.0f;
	buffer.bounceStrength = 2.0;
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


	StageLightData(cmdList, scene);

	cmdList.ClearBuffer(FrameBuffer, Color::CornflowerBlue);
	{
		Atmosphere->RenderSkySphere(cmdList, CameraRef, SunRef);


		cmdList.SetBlendState(BlendState);
		cmdList.SetRasterizerState(RasterizerState);
		cmdList.GetDynamic()->SetDepthStencilState(DepthStencilState);

		FRenderUtil::DrawScreenQuad(cmdList);

		cmdList.SetShaderPipeline(TransluscencyCombineShader);
		FRenderUtil::DrawScreenQuad(cmdList);

	}
	cmdList.UnbindFrameBuffer();
}
void DRStageLighting::Finish(FRICommandList& cmdList, RStageInterface& rso)
{
	rso.Buffer[RS_SLOT_LIT_SCENE] = LitTexture;
}

void DRStageLighting::StageLightData(FRICommandList& cmdList, Scene* scene)
{

	auto DLightSystem = scene->RegisterSystem<FRenderDLight>(ECSExecutionFlag::USER_TICK);
	auto PLightSystem = scene->RegisterSystem<FRenderPLight>(ECSExecutionFlag::USER_TICK);
	auto SLightSystem = scene->RegisterSystem<FRenderSLight>(ECSExecutionFlag::USER_TICK);


	uint32 DLightNum = DLightSystem->Count();
	uint32 PLightNum = PLightSystem->Count();
	uint32 SLightNum = SLightSystem->Count();


	auto& CameraRef = scene->Camera.Component<CameraComponent>();
	auto& SunRef = scene->Sun.Component<DirectionalLight>();

	// Lighting Pass Constants

	FMatrix4 inverseView		= FMatrix4::Inverse(CameraRef.View);
	FMatrix4 inverseProjection	= FMatrix4::Inverse(CameraRef.Projection);

	cmdList.SetShaderPipeline(LightingShader);
	cmdList.StageResourcesLambda(DeferredConstantBuffer, [&](FRIMemoryMap& stageMemory)
		{
			stageMemory.Load(CameraRef.View);
			stageMemory.Load(CameraRef.Projection);
			stageMemory.Load(inverseView);
			stageMemory.Load(inverseProjection);
			stageMemory.Load(inverseView[3]);
			stageMemory.Load(0.1f);
			stageMemory.Load(300.0f);
			stageMemory.Load(0.0f);
			stageMemory.Load(0.0f);


			stageMemory.Load(DLightNum);
			stageMemory.Load(PLightNum);
			stageMemory.Load(SLightNum);
			stageMemory.Load(0);

		});

	// Cascade Data

	cmdList.StageResourcesLambda(CascadeCameraBuffer, [&](FRIMemoryMap& stageMem)
		{
			for (int i = 0; i < SM_CASCADES; i++)
			{
				FMatrix4 toView = SunRef.FrustumInfo[i].View * inverseView;
				FMatrix4 toLight = CameraRef.Projection * toView;

				stageMem.Load(toLight);
				stageMem.Load(SunRef.FrustumInfo[i].Depth);
			}
		});


	// Lights Data

	cmdList.StageResourcesLambda(DirectionalLightBuffer, [&](FRIMemoryMap& stageMem)
		{
			DLightSystem->ForEach([&](Entity en, DirectionalLight& light)
				{
					light.StageMemory(stageMem);
				});

		});

	cmdList.StageResourcesLambda(PointLightBuffer, [&](FRIMemoryMap& stageMem)
		{
			PLightSystem->ForEach([&](Entity en, PointLight& light)
				{
					light.StageMemory(stageMem);
				});

		});

	cmdList.StageResourcesLambda(SpotLightBuffer, [&](FRIMemoryMap& stageMem)
		{
			SLightSystem->ForEach([&](Entity en, SpotLight& light)
				{
					light.StageMemory(stageMem);
				});

		});
}