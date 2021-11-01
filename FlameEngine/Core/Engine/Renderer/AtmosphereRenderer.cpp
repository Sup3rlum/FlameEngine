#include "AtmosphereRenderer.h"




struct FAtmosphereConstantBuffer
{
	FVector4 LightDirection;
	FVector3 RayleighScatteringCoeff;
	float MieScatteringCoeff;

	float RayleighScale;
	float MieScale;
	float MieDir;
	float SunIntensity;

	FMatrix4 InverseProjection;
	FMatrix4 InverseView;
};


void AtmosphereRenderer::CreateResources(FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());


	Shaders = FLocalContent::LoadFromLocal<ShaderLibrary>("shaders/atmosphere_dx.fslib", renderContext);
	renderSphere = FLocalContent::LoadFromLocal<Mesh>("models/cube.fl3d", renderContext);


	pipeline = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["AtmosphereDynamic"]);
	AtmosphereRasterizer = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);
	AtmosphereDepthState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);
	ConstantBuffer = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FAtmosphereConstantBuffer)));


	Atmosphere = AtmospherePresets::Earth;
	Scale = AtmosphereScale(8e3, 1.2e3, 0.99f);

}

void AtmosphereRenderer::RecreateResources(FRIContext* renderContext, FRIContext* prevContext)
{

}

void AtmosphereRenderer::RenderSkySphere(FRICommandList& cmdList, CameraComponent& cameraRef, DirectionalLight& dirLightRef)
{
	if (!Enabled)
		return;

	SetRenderStates(cmdList);

	cmdList.SetShaderPipeline(pipeline);
	{
		FAtmosphereConstantBuffer buffer;
		buffer.LightDirection.rgb = FVector3::Normalize(dirLightRef.Direction);


		buffer.RayleighScatteringCoeff = Atmosphere.RayleighScatteringCoefficient;
		buffer.MieScatteringCoeff = Atmosphere.MieScatteringCoeffcient;

		buffer.RayleighScale = Scale.RayleighScale;
		buffer.MieScale = Scale.MieScale;
		buffer.MieDir = Scale.GFactor;
		buffer.SunIntensity = Atmosphere.SunIntensity;
		buffer.InverseProjection = FMatrix4::Inverse(cameraRef.Projection);
		buffer.InverseView = FMatrix4::Inverse(cameraRef.View);

		FRIUpdateDescriptor dataStage(&buffer, 0, sizeof(FAtmosphereConstantBuffer));

		cmdList.UniformBufferSubdata(ConstantBuffer, dataStage);
	}

	/*cmdList.SetGeometrySource(renderSphere.VertexBuffer);
	cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, renderSphere.IndexBuffer->IndexCount, EFRIIndexType::UInt32, renderSphere.IndexBuffer);*/

	FRenderUtil::DrawScreenQuad(cmdList);

}

void AtmosphereRenderer::SetRenderStates(FRICommandList& cmdList)
{
	cmdList.SetShaderUniformBuffer(7, ConstantBuffer);

	cmdList.SetRasterizerState(AtmosphereRasterizer);
	cmdList.GetDynamic()->SetDepthStencilState(AtmosphereDepthState);
}