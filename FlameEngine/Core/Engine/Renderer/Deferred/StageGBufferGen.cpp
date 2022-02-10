#include "RenderStages.h"



void DRStageGBuffer::CreateResources(ShaderLibrary& Shaders, FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());

	// Create GBuffer Textures

	Viewport = renderContext->GetViewport();
	IVector2 viewportSize = Viewport.Size;

	//Depth				= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::R32F);
	Albedo				= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);
	Normal				= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);
	MetallicRoughness	= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);
	Emissive			= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);


	// Create Filtering params buffer
	FArray<FTextureParameterBufferParameter> paramBuffer1;
	paramBuffer1.Add(FTextureParameterBufferParameter(EFRITextureParamName::MinFilter, EFRITextureFilterMode::Bilinear));
	paramBuffer1.Add(FTextureParameterBufferParameter(EFRITextureParamName::MagFilter, EFRITextureFilterMode::Bilinear));

	//cmdList.SetTextureParameterBuffer(Depth, FRITextureParameterBuffer(paramBuffer1));
	cmdList.SetTextureParameterBuffer(Albedo, FRITextureParameterBuffer(paramBuffer1));
	cmdList.SetTextureParameterBuffer(Normal, FRITextureParameterBuffer(paramBuffer1));
	cmdList.SetTextureParameterBuffer(MetallicRoughness, FRITextureParameterBuffer(paramBuffer1));
	cmdList.SetTextureParameterBuffer(Emissive, FRITextureParameterBuffer(paramBuffer1));


	FArray<FRIFrameBufferAttachment> geomGenAttachments;
	//geomGenAttachments.Add(FRIFrameBufferAttachment(Depth));
	geomGenAttachments.Add(FRIFrameBufferAttachment(Normal));
	geomGenAttachments.Add(FRIFrameBufferAttachment(Albedo));
	geomGenAttachments.Add(FRIFrameBufferAttachment(MetallicRoughness));
	geomGenAttachments.Add(FRIFrameBufferAttachment(Emissive));

	
	FrameBuffer = cmdList.GetDynamic()->CreateFrameBuffer(geomGenAttachments, true);


	/* Skinned Geometry Gen Pipeline */
	GShader			= cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["GBufferGen"]);
	GSkinnedShader	= cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["GBufferGenSkinned"]);


	voxelAlbedo = cmdList.GetDynamic()->CreateTexture3D(256, 256, 256, EFRITextureFormat::RGBA8UNORM);
	voxelNormal = cmdList.GetDynamic()->CreateTexture3D(256, 256, 256, EFRITextureFormat::RGBA8UNORM);
	voxelEmission = cmdList.GetDynamic()->CreateTexture3D(256, 256, 256, EFRITextureFormat::RGBA8UNORM);


	TransformBuffer = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FTransformBufferStruct)));

	CameraMatrixBuffer	= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(CameraComponent)));
	TransformBuffer		= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FTransformBufferStruct)));
	JointBuffer			= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FMatrix4) * 32));
	MaterialBuffer			= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FMaterialBufferStruct)));

	BlendState = cmdList.GetDynamic()->CreateBlendState(EFRIAlphaBlend::Src, EFRIAlphaBlend::OneMinusSrc);
	DepthStencilState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::True, EFRIBool::False);
	RasterizerState = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);
}
void DRStageGBuffer::RecreateResources(ShaderLibrary& Shaders, FRIContext* context, FRIContext* previousContext)
{

}


void DRStageGBuffer::Prepare(FRICommandList& cmdList, RStageInterface& input)
{
	cmdList.SetBlendState(BlendState);
	cmdList.SetRasterizerState(RasterizerState);
	cmdList.GetDynamic()->SetDepthStencilState(DepthStencilState);


	cmdList.SetShaderUniformBuffer(UBO_SLOT_CAMERA, CameraMatrixBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_TRANSFORM, TransformBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_JOINTS, JointBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_MATERIAL, MaterialBuffer);

}
void DRStageGBuffer::SubmitPass(FRICommandList& cmdList, Scene* scene)
{
	Geometry = scene->RegisterSystem<FRenderSystemGeom>(ECSExecutionFlag::USER_TICK);
	SkinnedGeometry = scene->RegisterSystem<FRenderSystemSkinnedGeom>(ECSExecutionFlag::USER_TICK);


	CameraComponent& CameraRef = scene->Camera.Component<CameraComponent>();

	cmdList.StageResourcesLambda(CameraMatrixBuffer, [&](FRIMemoryMap& stageMem)
		{
			stageMem.Load(CameraRef);
		});


	cmdList.SetViewport(Viewport);
	cmdList.GetDynamic()->ClearBufferAndSetUAVs(FrameBuffer, Color::Transparent, new FRITexture3D* [] {voxelAlbedo, voxelNormal, voxelEmission}, 3);
	{

		/* Static Geometry */

		cmdList.SetShaderPipeline(GShader);


		for (auto& geom : scene->SceneLevel.LevelGeometry.Leafs)
		{

			/* Stage Samplers */
			for (int map = 0; map < (uint32)EMaterialMap::MAX_MAPS; map++)
			{
				FRITexture2D* textureMap = geom->Material.GetMap((EMaterialMap)map).Handle;
				if (textureMap)
				{
					cmdList.SetShaderSampler(FUniformSampler(map, textureMap));
				}
			}

			cmdList.StageResourcesLambda(MaterialBuffer, [&](FRIMemoryMap& stageMem)
				{
					geom->Material.StageMemory(stageMem);
				});
			cmdList.StageResourcesLambda(TransformBuffer, [&](FRIMemoryMap& stageMem)
				{
					stageMem.Load(FMatrix4::Identity());
					stageMem.Load(FMatrix4::Identity());
				});

			cmdList.SetGeometrySource(geom->VertexBuffer);
			cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, geom->IndexBuffer->IndexCount, EFRIIndexType::UInt32, geom->IndexBuffer);
		}


		Geometry->ForEach([&](Entity ent, Mesh& mesh, Material& material, FTransform& transformComponent)
			{

				if (material.Properties.HasTransluscent)
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

				/* Stage emission properties */
				cmdList.StageResourcesLambda(MaterialBuffer, [&](FRIMemoryMap& stageMem)
					{
						material.StageMemory(stageMem);
					});

				/* Draw */

				cmdList.SetGeometrySource(mesh.VertexBuffer);
				cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, mesh.IndexBuffer->IndexCount, EFRIIndexType::UInt32, mesh.IndexBuffer);
			}

		);


		/* Skinned Models */

		cmdList.SetShaderPipeline(GSkinnedShader);
		SkinnedGeometry->ForEach([&](Entity ent, SkinnedMesh& skinnedMesh, Material& material, FTransform& transformComponent)
			{
				if (material.Properties.HasTransluscent)
					return;

				/* Stage DataBuffer */
				cmdList.StageResourcesLambda(TransformBuffer, [&](FRIMemoryMap& stageMem)
					{
						FMatrix4 tmpWorld = transformComponent.GetMatrix();

						stageMem.Load(tmpWorld);
						stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

					});
				cmdList.StageResourcesLambda(JointBuffer, [&](FRIMemoryMap& stageMem)
					{
						const FArray<FMatrix4>& jointMatrices = skinnedMesh.MeshSkeleton.GetJointTransforms();
						stageMem.Load(jointMatrices.Begin(), jointMatrices.ByteSize());

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


				/* Stage emission properties */
				cmdList.StageResourcesLambda(MaterialBuffer, [&](FRIMemoryMap& stageMem)
					{
						material.StageMemory(stageMem);
					});

				/* Draw */

				cmdList.SetGeometrySource(skinnedMesh.VertexBuffer);
				cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, skinnedMesh.IndexBuffer->IndexCount, EFRIIndexType::UInt32, skinnedMesh.IndexBuffer);
			}

		);


	}
	cmdList.UnbindFrameBuffer();
}
void DRStageGBuffer::Finish(FRICommandList& cmdList, RStageInterface& rso)
{
	rso.Buffer[RS_SLOT_DEPTH] = FrameBuffer->GetDepthBuffer();
	rso.Buffer[RS_SLOT_NORMAL] = Normal;
	rso.Buffer[RS_SLOT_ALBEDO] = Albedo;
	rso.Buffer[RS_SLOT_METALLIC_ROUGHNESS] = MetallicRoughness;
	rso.Buffer[RS_SLOT_EMISSIVE] = Emissive;
}