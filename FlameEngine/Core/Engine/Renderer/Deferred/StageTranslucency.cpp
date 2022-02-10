#include "RenderStages.h"




struct FDirLightInfo : FRIArrayInterface
{
	FVector4 Direction;
	FVector4 Info;
};


void DRStageTransluscency::CreateResources(ShaderLibrary& library, FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());

	// Create GBuffer Textures
	IVector2 viewportSize = renderContext->GetViewport().Size;

	ObjectPipeline = cmdList.GetDynamic()->CreateShaderPipeline(library.Modules["Transluscent"]);


	Output = cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA32F);
	FrameBuffer = cmdList.GetDynamic()->CreateFrameBuffer({ FRIFrameBufferAttachment(Output) }, true);

	TransformBuffer = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FTransformBufferStruct)));
	DirLightBuffer = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FDirLightInfo)));

	BlendState = cmdList.GetDynamic()->CreateBlendState(EFRIAlphaBlend::Src, EFRIAlphaBlend::OneMinusSrc);
	DepthStencilState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::True, EFRIBool::False);
	RasterizerState = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);
}
void DRStageTransluscency::RecreateResources(ShaderLibrary& library, FRIContext* context, FRIContext* previousContext)
{

}


void DRStageTransluscency::Prepare(FRICommandList& cmdList, RStageInterface& input)
{
	cmdList.SetBlendState(BlendState);
	cmdList.SetRasterizerState(RasterizerState);
	cmdList.GetDynamic()->SetDepthStencilState(DepthStencilState);


	cmdList.SetShaderUniformBuffer(UBO_SLOT_TRANSFORM, TransformBuffer);
	//cmdList.SetShaderUniformBuffer(UBO_SLOT_DIR_LIGHT, DirLightBuffer);


	cmdList.ClearBuffer(FrameBuffer, Color::Transparent);
	cmdList.GetDynamic()->CopyTexture(input.GetResource<FRITexture2D>(RS_SLOT_DEPTH), FrameBuffer->GetDepthBuffer());
}
void DRStageTransluscency::SubmitPass(FRICommandList& cmdList, Scene* scene)
{

	DirectionalLight& SunRef = scene->Sun.Component<DirectionalLight>();

	auto Geometry = scene->RegisterSystem<FRenderSystemGeom>(ECSExecutionFlag::USER_TICK);



	/* Stage DataBuffer */
	cmdList.StageResourcesLambda(DirLightBuffer, [&] (FRIMemoryMap& stageMemory)
		{
			stageMemory.Load(SunRef.Direction);
			stageMemory.Load(SunRef.Color.rgb);
			stageMemory.Load(SunRef.Intensity);

		});


	cmdList.SetShaderPipeline(ObjectPipeline);
	Geometry->ForEach([&](Entity ent, Mesh& mesh, Material& material, FTransform& transformComponent)
		{

			if (!material.Properties.HasTransluscent)
				return;

			/* Stage DataBuffer */
			cmdList.StageResourcesLambda(TransformBuffer, [&](FRIMemoryMap& stageMem)
				{
					FMatrix4 tmpWorld = transformComponent.GetMatrix();

					stageMem.Load(tmpWorld);
					stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

				});


			/* Stage Samplers */
			for (int map = 0; map < (uint32)EMaterialMap::MAX_MAPS; map++)
			{
				FRITexture2D* textureMap = material.GetMap((EMaterialMap)map).Handle;
				if (textureMap)
				{
					cmdList.SetShaderSampler(FUniformSampler(map, textureMap));
				}
			}


			/* Draw */

			cmdList.SetGeometrySource(mesh.VertexBuffer);
			cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, mesh.IndexBuffer->IndexCount, EFRIIndexType::UInt32, mesh.IndexBuffer);
		}

	);

	for (auto& pSystemPair : scene->ParticleSystems)
	{
		pSystemPair.Value->Prepare(cmdList);
		pSystemPair.Value->RenderSystem(cmdList, pSystemPair.Key);
	}


	cmdList.UnbindFrameBuffer();
}
void DRStageTransluscency::Finish(FRICommandList& cmdList, RStageInterface& output)
{
	output.Buffer[RS_SLOT_TRANSLUSCENCY] = Output;
}