#include "RenderStages.h"




void DRStageEnvironment::CreateResources(ShaderLibrary& Shaders, FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());
	IVector2 viewportSize = renderContext->GetViewport().Size;





	BlendState = cmdList.GetDynamic()->CreateBlendState(EFRIAlphaBlend::Src, EFRIAlphaBlend::OneMinusSrc);
	DepthStencilState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);
	RasterizerState = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);

}



void DRStageEnvironment::RecreateResources(ShaderLibrary& Shaders, FRIContext* renderContext, FRIContext* prevContext)
{

}

void DRStageEnvironment::Prepare(FRICommandList& cmdList, RStageInterface& input)
{

	/*
	cmdList.SetBlendState(BlendState);
	cmdList.SetRasterizerState(RasterizerState);
	cmdList.GetDynamic()->SetDepthStencilState(DepthStencilState);

	cmdList.SetShaderUniformBuffer(UBO_SLOT_COMBINE, LightingConstantBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_CASCADE_CAMERA, CascadeCameraBuffer);

	cmdList.SetShaderSampler(FUniformSampler(0, static_cast<FRITexture2D*>(input.Buffer[RS_SLOT_DEPTH])));*/

}
void DRStageEnvironment::SubmitPass(FRICommandList& cmdList, Scene* scene)
{

	/*
	CameraComponent& CameraRef = scene->Camera.Component<CameraComponent>();
	FTransform& CameraPosRef = scene->Camera.Component<FTransform>();
	DirectionalLight& SunRef = scene->Sun.Component<DirectionalLight>();



	cmdList.ClearBuffer(FrameBuffer, Color::CornflowerBlue);
	{
		//atmosphereRenderer.RenderSkySphere(cmdList, CameraRef, SunRef);

		cmdList.SetShaderPipeline(LightingShader);

		cmdList.StageResources([&]
			{
				FCombineBufferStruct buffer;
				buffer.Direction.rgb = SunRef.Direction;
				buffer.Info = FVector4(SunRef.LightColor.rgb, SunRef.Intensity);
				buffer.InverseView = FMatrix4::Inverse(CameraRef.View);
				buffer.InverseProjection = FMatrix4::Inverse(CameraRef.Projection);
				buffer.AmbienceFactor = 0.1;
				buffer.Exposure = 1.0f;

				FRIUpdateDescriptor dataStage(&buffer, 0, sizeof(FCombineBufferStruct));
				FRIUpdateDescriptor cascadeStage(SunRef.FrustumInfo.Begin(), 0, sizeof(FViewFrustumInfo) * SM_CASCADES);

				cmdList.UniformBufferSubdata(LightingConstantBuffer, dataStage);
				cmdList.UniformBufferSubdata(CascadeCameraBuffer, cascadeStage);

			});

		FRenderUtil::DrawScreenQuad(cmdList);
	}
	cmdList.UnbindFrameBuffer();*/
}
void DRStageEnvironment::Finish(FRICommandList& cmdList, RStageInterface& rso)
{
	//rso.Buffer[RS_SLOT_LIT_SCENE] = LitTexture;
}