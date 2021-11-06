#include "DeferredRenderer.h"


#include "Core/Framework/IO/FileStream.h"
#include "../ContentSystem/Client/AssetImportScripts/Material.h"


#define UBO_SLOT_CAMERA 0
#define UBO_SLOT_TRANSFORM 1
#define UBO_SLOT_CASCADE_CAMERA 2
#define UBO_SLOT_JOINTS 3
#define UBO_SLOT_COMBINE 4





struct FCameraBufferStruct
{
	FViewMatrix View;
	FProjectionMatrix Projection;
};


void DeferredRenderer::CreateResources(FRIContext* renderContext)
{

	FRICommandList cmdList(renderContext->GetFRIDynamic());



	if (renderContext->RenderFramework == EFRIRendererFramework::DX11)
	{
		Shaders = FLocalContent::LoadFromLocal<ShaderLibrary>("shaders/deferred_dx.fslib", renderContext);
	}
	else
	{
		Shaders = FLocalContent::LoadFromLocal<ShaderLibrary>("shaders/deferred.fslib", renderContext);
	}


	/* Create Render Stage resources */


	FRenderUtil::VertexBuffer = cmdList.GetDynamic()->CreateVertexBuffer(
		4, 0, FRICreationDescriptor(
			FRenderUtil::quadVData.Begin(),
			FRenderUtil::quadVData.ByteSize()
		));


	FRIVertexShader* scrQuadSignatureShader = NULL;// cmdList.GetDynamic()->CreateVertexShader(IOFileStream("shaders/signature/dx/bin/CombineQuad.signature.cso").ReadBytes());
	if (renderContext->RenderFramework == EFRIRendererFramework::DX11)
	{
		scrQuadSignatureShader = cmdList.GetDynamic()->CreateVertexShader(IOFileStream("shaders/signature/dx/bin/CombineQuad.signature.cso").ReadBytes());
	}


	FArray<FRIVertexDeclarationDesc> LayoutDesc;
	LayoutDesc.Add(FRIVertexDeclarationDesc(FRenderUtil::VertexDeclComp, 0));

	FRenderUtil::VertexDeclaration = cmdList.GetDynamic()->CreateVertexDeclaration(LayoutDesc, scrQuadSignatureShader);
	cmdList.GetDynamic()->AttachVertexDeclaration(FRenderUtil::VertexBuffer, FRenderUtil::VertexDeclaration);

	FRenderUtil::IndexBuffer = cmdList.GetDynamic()->CreateIndexBuffer(
		6, 0, FRICreationDescriptor(
			FRenderUtil::quadElementData.Begin(),
			FRenderUtil::quadElementData.ByteSize()
		)
	);


	//FRenderUtil::ScreenSpaceMatrix = FOrthographicMatrix(0, viewportSize.x, viewportSize.y, 0, 0.0f, 5.0f);


	RenderStages.Add(new DRStageShadowmap());
	RenderStages.Add(new DRStageGBuffer());
	RenderStages.Add(new DRStageAOGen());
	RenderStages.Add(new DRStageTransluscency());
	RenderStages.Add(new DRStageLighting());
	RenderStages.Add(new DRStageSMAA());
	RenderStages.Add(new DRStagePostProcess());


	for (auto stage : RenderStages)
	{
		stage->CreateResources(Shaders, renderContext);
	}


	

	atmosphereRenderer.CreateResources(renderContext);

	/*bvds = new BoundingVolumeDebugService();
	bvds->CreateResources(renderContext);*/
}

void DeferredRenderer::RecreateResources(FRIContext* renderContext, FRIContext* prevContext)
{

}



void DeferredRenderer::BeginRender(FRICommandList& cmdList)
{



}

void DeferredRenderer::Render(FRICommandList& cmdList)
{


	RStageInterface RenderInterface;

	for (auto stage : RenderStages)
	{
		stage->Prepare(cmdList, RenderInterface);
		stage->SubmitPass(cmdList, scene);
		stage->Finish(cmdList, RenderInterface);
	}



	
	/*// Present
	cmdList.ClearBuffer(NULL, Color::Black);
	{
		cmdList.SetShaderPipeline();
		cmdList.SetShaderSampler(FUniformSampler(0, RenderInterface.GetResource<FRITexture2D>(RS_SLOT_FINAL)));

		FRenderUtil::DrawScreenQuad(cmdList);
	}*/

}
void DeferredRenderer::EndRender(FRICommandList& cmdList)
{


}


void DeferredRenderer::AttachToScene(Scene* scene)
{
	this->scene = scene;
}