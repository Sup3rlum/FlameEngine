#include "AtmosphereRenderer.h"


void AtmosphereRenderer::CreateResources(FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());


	FAnsiString vsData = IOFileStream("shaders/atmosphere/basic.vert").ReadAnsiFile();
	FAnsiString psData = IOFileStream("shaders/atmosphere/basic.frag").ReadAnsiFile();

	FResourceVertexShader* vs = cmdList.GetDynamic()->DynamicCreateVertexShader(vsData);
	FResourcePixelShader* ps = cmdList.GetDynamic()->DynamicCreatePixelShader(psData);

	pipeline = cmdList.GetDynamic()->DynamicCreateShaderPipeline
	(
		FResourceShaderPipelineCreationDescriptor(
			2,
			new FResourceShaderBase* [2]{ vs, ps }
		)
	);

	WorldLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(pipeline, "World");
	ViewLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(pipeline, "View");
	ProjLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(pipeline, "Projection");

	CameraPosLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(pipeline, "CameraPos");
	LightDirectionLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(pipeline, "LightDirection");



	FMeshLoader meshLoader(true, true);
	renderSphere = meshLoader.LoadFromLocal("models/sphere.fl3d", renderContext);
}

void AtmosphereRenderer::RenderSkySphere(FRICommandList& cmdList, CameraComponent& cameraRef, TransformComponent& cameraTransform)
{
	cmdList.SetBackCull(false);

	cmdList.SetShaderPipeline(pipeline);

	cmdList.SetShaderUniformParameter(FUniformParameter(WorldLoc, FTranslationMatrix(cameraTransform.Transform.Position)));
	cmdList.SetShaderUniformParameter(FUniformParameter(ViewLoc, cameraRef.View));
	cmdList.SetShaderUniformParameter(FUniformParameter(ProjLoc, cameraRef.Projection));

	cmdList.SetShaderUniformParameter(FUniformParameter(CameraPosLoc, cameraTransform.Transform.Position));
	cmdList.SetShaderUniformParameter(FUniformParameter(LightDirectionLoc, FVector3::Normalize(FVector3(-1, -1, -1))));

	cmdList.SetGeometrySource(renderSphere.VertexBuffer);
	cmdList.DrawPrimitivesIndexed((uint32)EFRIPrimitiveType::Triangles, renderSphere.IndexBuffer->IndexCount, (uint32)EFRIIndexType::UInt32, renderSphere.IndexBuffer);


	cmdList.SetBackCull(true);

}