#include "RenderStages.h"




void DRStageShadowmap::CreateResources(ShaderLibrary& Shaders, FRIContext* renderContext)
{

	FRICommandList cmdList(renderContext->GetFRIDynamic());

	/* Shadowmap Pipeline */
	Shader			= cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["Shadowmap"]);
	SkinnedShader	= cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["ShadowmapSkinned"]);


	Viewport = FViewportRect(0, 0, 4096, 4096);
	Output = cmdList.GetDynamic()->CreateTexture2DArray(Viewport.Width, Viewport.Height, SM_CASCADES, EFRITextureFormat::RG32F, FRIColorDataFormat(EFRIChannels::RG, EFRIPixelStorage::Float));

	ShadowmapFrameBuffer = cmdList.GetDynamic()->CreateFrameBuffer(FRIFrameBufferArrayAttachment(Output), true);


	CameraMatrixBuffer	= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(CameraComponent)));
	TransformBuffer		= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FTransformBufferStruct)));
	JointBuffer			= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FMatrix4) * 32));


	BlendState = cmdList.GetDynamic()->CreateBlendState(EFRIAlphaBlend::Src, EFRIAlphaBlend::OneMinusSrc);
	DepthStencilState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::True, EFRIBool::False);
	RasterizerState = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::None, EFRIFillMode::Solid);
}
void DRStageShadowmap::RecreateResources(ShaderLibrary& Shaders, FRIContext* context, FRIContext* previousContext)
{

}


void DRStageShadowmap::Prepare(FRICommandList& cmdList, RStageInterface& input)
{
	cmdList.SetBlendState(BlendState);
	cmdList.SetRasterizerState(RasterizerState);
	cmdList.GetDynamic()->SetDepthStencilState(DepthStencilState);


	cmdList.SetShaderUniformBuffer(UBO_SLOT_CAMERA, CameraMatrixBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_TRANSFORM, TransformBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_JOINTS, JointBuffer);

}
void DRStageShadowmap::SubmitPass(FRICommandList& cmdList, Scene* scene)
{
	SMGeometry = scene->RegisterSystem<FRenderSystemSM>(ECSExecutionFlag::USER_TICK);
	SkinnedSMGeometry = scene->RegisterSystem<FRenderSystemSkinnedSM>(ECSExecutionFlag::USER_TICK);

	CameraComponent& CameraRef = scene->Camera.Component<CameraComponent>();
	FTransform& CameraPosRef = scene->Camera.Component<FTransform>();
	DirectionalLight& SunRef = scene->Sun.Component<DirectionalLight>();


	cmdList.SetViewport(Viewport);
	cmdList.ClearBuffer(ShadowmapFrameBuffer, Color::White);
	{
		for (int i = 0; i < SM_CASCADES; i++)
		{
			cmdList.SetFramebufferTextureLayer(ShadowmapFrameBuffer, i);


			// Stage cascade camera buffer
			cmdList.StageResourcesLambda(CameraMatrixBuffer, [&](FRIMemoryMap& stageMem)
				{
					stageMem.Load(&SunRef.FrustumInfo[i].View, sizeof(CameraComponent));
				});

			cmdList.StageResourcesLambda(TransformBuffer, [&](FRIMemoryMap& stageMem)
				{
					stageMem.Load(FMatrix4::Identity());
					stageMem.Load(FMatrix4::Identity());

				});


			/*  Static Shadowed Scene  */
			cmdList.SetShaderPipeline(Shader);


			for (auto& geom : scene->SceneLevel.LevelGeometry.Leafs)
			{
				cmdList.SetGeometrySource(geom->VertexBuffer);
				cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, geom->IndexBuffer->IndexCount, EFRIIndexType::UInt32, geom->IndexBuffer);
			}


			SMGeometry->ForEach([&](Entity ent, Mesh& mesh, FTransform& transformComponent)
				{
					cmdList.StageResourcesLambda(TransformBuffer, [&](FRIMemoryMap& stageMem)
						{
							FMatrix4 tmpWorld = transformComponent.GetMatrix();

							stageMem.Load(tmpWorld);
							stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

						});

					cmdList.SetGeometrySource(mesh.VertexBuffer);
					cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, mesh.IndexBuffer->IndexCount, EFRIIndexType::UInt32, mesh.IndexBuffer);

				});


			/*  Skinned Shadowed Scene  */
			cmdList.SetShaderPipeline(SkinnedShader);
			SkinnedSMGeometry->ForEach([&](Entity ent, SkinnedMesh& mesh, FTransform& transformComponent)
				{
					cmdList.StageResourcesLambda(TransformBuffer, [&](FRIMemoryMap& stageMem)
						{
							FMatrix4 tmpWorld = transformComponent.GetMatrix();

							stageMem.Load(tmpWorld);
							stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

						});

					cmdList.StageResourcesLambda(JointBuffer, [&](FRIMemoryMap& stageMem)
						{
							const FArray<FMatrix4>& jointMatrices = mesh.MeshSkeleton.GetJointTransforms();
							stageMem.Load(jointMatrices.Begin(), jointMatrices.ByteSize());

						});

					cmdList.SetGeometrySource(mesh.VertexBuffer);
					cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, mesh.IndexBuffer->IndexCount, EFRIIndexType::UInt32, mesh.IndexBuffer);

				});

			/*
			for (auto& pSystemPair : scene->ParticleSystems)
			{
				pSystemPair.Value->RenderSystem(cmdList, pSystemPair.Key);
			}*/
		}


		//cmdList.FlushMipMaps(Output);
	}
	cmdList.UnbindFrameBuffer();
}
void DRStageShadowmap::Finish(FRICommandList& cmdList, RStageInterface& rso)
{
	rso.Buffer[RS_SLOT_SHADOWMAP] = Output;
}