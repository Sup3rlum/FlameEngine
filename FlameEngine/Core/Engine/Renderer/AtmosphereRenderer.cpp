#include "AtmosphereRenderer.h"


struct FAtmosphereConstantBuffer
{
	FMatrix4 World;
	FVector4 CameraPosition;
	FVector4 LightDirection;
};


void AtmosphereRenderer::CreateResources(FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());


	/*FMeshLoader meshLoader(true, true);
	FShaderLibLoader shaderLibLoader(false, false);

	Shaders = shaderLibLoader.LoadFromLocal("shaders/atmosphere.fslib", renderContext);

	pipeline = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["AtmosphereBasic"]);


	renderSphere = meshLoader.LoadFromLocal("models/sphere.fl3d", renderContext);*/
}

void AtmosphereRenderer::RenderSkySphere(FRICommandList& cmdList, CameraComponent& cameraRef, FTransform cameraTransform)
{
	/*cmdList.SetBackCull(false);

	cmdList.SetShaderPipeline(pipeline);


	FAtmosphereConstantBuffer buffer;
	buffer.World = FTranslationMatrix(cameraTransform.Transform.Position);
	buffer.CameraPosition.rgb = cameraTransform.Transform.Position;
	buffer.LightDirection.rgb = FVector3::Normalize(FVector3(-1, -1, -1));


	cmdList.SetGeometrySource(renderSphere.VertexBuffer);
	cmdList.DrawPrimitivesIndexed((uint32)EFRIPrimitiveType::Triangles, renderSphere.IndexBuffer->IndexCount, (uint32)EFRIIndexType::UInt32, renderSphere.IndexBuffer);


	cmdList.SetBackCull(true);*/

}