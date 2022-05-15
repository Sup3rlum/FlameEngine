#include "AtmosphereRenderer.h"
#include "Core/Engine/ContentSystem/Client/LocalAssetManager.h"



struct FAtmosphereConstantBuffer
{
	FMatrix4 InverseProjection;
	FMatrix4 InverseView;

	FVector4 LightDirection;
	FVector3 RayleighScatteringCoeff;
	float MieScatteringCoeff;

	float RayleighScale;
	float MieScale;
	float MieDir;
	float SunIntensity;

	float PlanetRadius;
	float AtmosphereThickness;
	int PrimarySteps;
	int SecondarySteps;
};


void AtmosphereRenderer::CreateResources(FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());


	FAssetManager Content;
	Content.Connect("./Assets/");
	Content.RenderContext = renderContext;
	Shaders = Content.Load<ShaderLibrary>("Shaders/atmosphere_dx.fslib");

	pipeline = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["AtmosphereDynamic"]);
	AtmRasterizer = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);
	AtmDepthState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);
	SettingsBuffer.GPU = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FAtmosphereConstantBuffer)));
}

void AtmosphereRenderer::RecreateResources(FRIContext* renderContext, FRIContext* prevContext)
{

}

void AtmosphereRenderer::RenderSkySphere(FRICommandList& cmdList, const Camera& cameraRef, const DirectionalLight& dirLight)
{
	if (!Enabled)
		return;

	SetRenderStates(cmdList);

	cmdList.SetShaderPipeline(pipeline);
	{
		FAtmosphereConstantBuffer buffer;

		buffer.InverseProjection	= FMatrix4::Inverse(cameraRef.Projection);
		buffer.InverseView			= FMatrix4::Inverse(cameraRef.View);

		buffer.LightDirection.rgb	= FVector3::Normalize(dirLight.Direction);

		buffer.RayleighScatteringCoeff  = ThisProperty(RayleighScatteringCoefficient);
		buffer.MieScatteringCoeff		= ThisProperty(MieScatteringCoeffcient);

		buffer.RayleighScale			= ThisProperty(RayleighScale);
		buffer.MieScale					= ThisProperty(MieScale);
		buffer.MieDir					= ThisProperty(GFactor);
		buffer.SunIntensity				= ThisProperty(SunIntensity);

		buffer.PlanetRadius				= ThisProperty(PlanetRadius);
		buffer.AtmosphereThickness		= ThisProperty(AtmosphereThickness);
		buffer.PrimarySteps				= ThisProperty(PrimarySteps);
		buffer.SecondarySteps			= ThisProperty(SecondarySteps);



		cmdList.UniformBufferSubdata(SettingsBuffer.GPU, FRIUpdateDescriptor(&buffer, 0, sizeof(FAtmosphereConstantBuffer)));
	}

	FRenderUtil::DrawScreenQuad(cmdList);

}

void AtmosphereRenderer::SetRenderStates(FRICommandList& cmdList)
{
	cmdList.SetShaderUniformBuffer(9, SettingsBuffer.GPU);

	cmdList.SetRasterizerState(AtmRasterizer);
	cmdList.SetDepthStencilState(AtmDepthState);
}