#include "RenderStages.h"

#include "../AtmosphereRenderer.h"
#include "BRDF.h"

struct FCombineBufferStruct : FRIArrayInterface
{
	FMatrix4 InverseView;
	FMatrix4 InverseProjection;
	FVector4 Direction;
	FVector4 Info;
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
	LightingConstantBuffer	= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FCombineBufferStruct)));
	PointLightBuffer		= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(PointLight) * 4));

	BlendState = cmdList.GetDynamic()->CreateBlendState(EFRIAlphaBlend::Src, EFRIAlphaBlend::OneMinusSrc);
	DepthStencilState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);
	RasterizerState = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);

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

	cmdList.SetShaderUniformBuffer(UBO_SLOT_COMBINE, LightingConstantBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_CASCADE_CAMERA, CascadeCameraBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_POINT_LIGHT, PointLightBuffer);

	cmdList.SetShaderSampler(FUniformSampler(0, static_cast<FRITexture2D*>(input.Buffer[RS_SLOT_DEPTH])));
	cmdList.SetShaderSampler(FUniformSampler(1, static_cast<FRITexture2D*>(input.Buffer[RS_SLOT_NORMAL])));
	cmdList.SetShaderSampler(FUniformSampler(2, static_cast<FRITexture2D*>(input.Buffer[RS_SLOT_ALBEDO])));
	cmdList.SetShaderSampler(FUniformSampler(3, static_cast<FRITexture2D*>(input.Buffer[RS_SLOT_METALLIC_ROUGHNESS])));
	cmdList.SetShaderSampler(FUniformSampler(5, static_cast<FRITexture2D*>(input.Buffer[RS_SLOT_AO])));
	cmdList.SetShaderSampler(FUniformSampler(6, static_cast<FRITexture2D*>(input.Buffer[RS_SLOT_EMISSIVE])));
	cmdList.SetShaderSampler(FUniformSampler(4, static_cast<FRITexture2DArray*>(input.Buffer[RS_SLOT_SHADOWMAP])));
	cmdList.SetShaderSampler(FUniformSampler(7, BRDFLut));
	cmdList.SetShaderSampler(FUniformSampler(8, input.GetResource<FRITexture2D>(RS_SLOT_TRANSLUSCENCY)));
}
void DRStageLighting::SubmitPass(FRICommandList& cmdList, Scene* scene)
{
	CameraComponent& CameraRef = scene->Camera.Component<CameraComponent>();
	FTransform& CameraPosRef = scene->Camera.Component<FTransform>();
	DirectionalLight& SunRef = scene->Sun.Component<DirectionalLight>();



	cmdList.ClearBuffer(FrameBuffer, Color::CornflowerBlue);
	{
		Atmosphere->RenderSkySphere(cmdList, CameraRef, SunRef);


		cmdList.SetBlendState(BlendState);
		cmdList.SetRasterizerState(RasterizerState);
		cmdList.GetDynamic()->SetDepthStencilState(DepthStencilState);

		cmdList.SetShaderPipeline(LightingShader);
		cmdList.StageResources([&]
			{
				FCombineBufferStruct buffer;
				buffer.Direction.rgb = SunRef.Direction;
				buffer.Info = FVector4(FVector3(1), SunRef.Intensity);
				buffer.InverseView = FMatrix4::Inverse(CameraRef.View);
				buffer.InverseProjection = FMatrix4::Inverse(CameraRef.Projection);
				buffer.AmbienceFactor	= 0.1;
				buffer.Exposure			= 1.0f;

				FRIUpdateDescriptor dataStage(&buffer, 0, sizeof(FCombineBufferStruct));
				FRIUpdateDescriptor cascadeStage(SunRef.FrustumInfo.Begin(), 0, sizeof(FViewFrustumInfo) * SM_CASCADES);
				FRIUpdateDescriptor pointLightStage(&scene->pointLights, 0, sizeof(PointLight) * 4);

				cmdList.UniformBufferSubdata(LightingConstantBuffer, dataStage);
				cmdList.UniformBufferSubdata(CascadeCameraBuffer, cascadeStage);
				cmdList.UniformBufferSubdata(PointLightBuffer, pointLightStage);

			});

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