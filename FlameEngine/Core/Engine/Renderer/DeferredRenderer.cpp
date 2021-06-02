#include "DeferredRenderer.h"


#include "Core/Framework/IO/FileStream.h"
#include "../ContentSystem/Client/AssetImportScripts/Material.h"


#define UBO_SLOT_CAMERA 0
#define UBO_SLOT_TRANSFORM 1
#define UBO_SLOT_CASCADE_CAMERA 2
#define UBO_SLOT_JOINTS 3
#define UBO_SLOT_COMBINE 4


FStaticArray<FVector3, 8> cubeCorners =
{
	FVector3(1.0f, -1.0f, -1.0f),	 // llb
	FVector3(-1.0f, -1.0f, -1.0f), // lrb
	FVector3(-1.0f, 1.0f, -1.0f),  // urb
	FVector3(1.0f, 1.0f, -1.0f),	 // ulb


	FVector3(1.0f, -1.0f, 1.0f),   // llf
	FVector3(-1.0f, -1.0f, 1.0f),  // lrf
	FVector3(-1.0f, 1.0f, 1.0f),   // urf
	FVector3(1.0f, 1.0f, 1.0f)	 // ulf
};


struct FCombineBufferStruct : FRIArrayInterface
{
	FMatrix4 InverseView;
	FMatrix4 InverseProjection;
	FVector4 Direction;
	FVector4 Info;
};


struct FCameraBufferStruct
{
	FViewMatrix View;
	FProjectionMatrix Projection;
};
struct FTransformBufferStruct
{
	FMatrix4 World;
	FMatrix4 WorldInverseTranspose;
};


void DeferredRenderer::CreateResources(FRIContext* renderContext)
{

	this->renderContext = renderContext;

	FRICommandList cmdList(renderContext->GetFRIDynamic());
	FShaderLibLoader shaderLibLoader(false, false);


	if (renderContext->RenderFramework == EFRIRendererFramework::DX11)
	{
		Shaders = shaderLibLoader.LoadFromLocal("shaders/deferred_dx.fslib", renderContext);
	}
	else
	{
		Shaders = shaderLibLoader.LoadFromLocal("shaders/deferred.fslib", renderContext);
	}


	FArray<FTextureParameterBufferParameter> paramBuffer1;

	paramBuffer1.Add(FTextureParameterBufferParameter(EFRITextureParamName::MinFilter, EFRITextureFilterMode::Bilinear));
	paramBuffer1.Add(FTextureParameterBufferParameter(EFRITextureParamName::MagFilter, EFRITextureFilterMode::Bilinear));
	//paramBuffer1.Add(FTextureParameterBufferParameter(EFRITextureParamName::AnisotropyLevel, 2.0f));

	/* Texture Buffer Gen */

	IVector2 viewportSize = renderContext->GetViewport().Size;

	BufferTextures.DepthBuffer		= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::R32F);
	BufferTextures.AlbedoBuffer		= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);
	BufferTextures.NormalBuffer		= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);
	BufferTextures.SpecularBuffer	= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);
	BufferTextures.SSAOBuffer		= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);



	ShadowmapPipeline.Viewport = FViewportRect(0, 0, 2048, 2048);


	BufferTextures.ShadowmapCascadeArray = cmdList.GetDynamic()->CreateTexture2DArray(ShadowmapPipeline.Viewport.Height, ShadowmapPipeline.Viewport.Width, 5, EFRITextureFormat::RG32F, FRIColorDataFormat(EFRIChannels::RG, EFRIPixelStorage::Float));


	cmdList.SetTextureParameterBuffer(BufferTextures.DepthBuffer, FRITextureParameterBuffer(paramBuffer1));
	cmdList.SetTextureParameterBuffer(BufferTextures.AlbedoBuffer, FRITextureParameterBuffer(paramBuffer1));
	cmdList.SetTextureParameterBuffer(BufferTextures.NormalBuffer, FRITextureParameterBuffer(paramBuffer1));
	cmdList.SetTextureParameterBuffer(BufferTextures.SpecularBuffer, FRITextureParameterBuffer(paramBuffer1));
	cmdList.SetTextureParameterBuffer(BufferTextures.SSAOBuffer, FRITextureParameterBuffer(paramBuffer1));
	cmdList.SetTextureParameterBuffer(BufferTextures.ShadowmapCascadeArray, FRITextureParameterBuffer(paramBuffer1));


	/* Create Render Stage resources */

	CreateGBufferGenStage();
	CreateHBAOStage();
	CreateShadowmapStage();
	CreateCombineStage();


	/* Create Constant buffers */
	cameraMatrixBuffer						= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(CameraComponent)));
	transformBuffer							= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FTransformBufferStruct)));
	ShadowmapPipeline.CascadeCameraBuffer	= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FViewFrustumInfo) * 5));
	jointBuffer								= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FMatrix4) * 32));
	CombinePipeline.ConstantBuffer			= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FCombineBufferStruct)));


	atmosphereRenderer.CreateResources(renderContext);




	/*
	FArray<FVertexComponent_Color> qdata =
	{
		FVertexComponent_Color(FVector3(-1,1,0), FVector3(0,1,0)),
		FVertexComponent_Color(FVector3(1,1,0), FVector3(1,1,0)),
		FVertexComponent_Color(FVector3(1,-1,0), FVector3(1,0,0)),
		FVertexComponent_Color(FVector3(-1,-1,0), FVector3(0,0,1))

	};

	FArray<FIndexComponent> qedata = { 0,2,1,0,3,2 };



	triangleVB = cmdList.GetDynamic()->CreateVertexBuffer(
		4, 0, FRICreationDescriptor(
			qdata.Begin(),
			qdata.ByteSize()
		));


	auto scrQuadSignatureShader = cmdList.GetDynamic()->CreateVertexShader(rendererShaders.Modules["Triangle"].Parts[EFRIResourceShaderType::Vertex].Memory);

	FArray<FRIVertexDeclarationComponent> declComp;

	declComp.Add(FRIVertexDeclarationComponent("POSITION", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 24, 0));
	declComp.Add(FRIVertexDeclarationComponent("COLOR", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 24, 12));


	auto vd = cmdList.GetDynamic()->CreateVertexDeclaration(declComp, scrQuadSignatureShader);
	cmdList.GetDynamic()->AttachVertexDeclaration(triangleVB, vd);

	triangleIB = cmdList.GetDynamic()->CreateIndexBuffer(
		6, 0, FRICreationDescriptor(
			qedata.Begin(),
			qedata.ByteSize()
		)
	);

	trianglePipe = cmdList.GetDynamic()->CreateShaderPipeline(rendererShaders.Modules["Triangle"]);*/

}

/****************************************
*			GBufferGen Stage
*****************************************/



void DeferredRenderer::CreateGBufferGenStage()
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());

	GeomGenPipeline.Handle = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["GBufferGen"]);

	FArray<FRIFrameBufferAttachment> geomGenAttachments;
	geomGenAttachments.Add(FRIFrameBufferAttachment(BufferTextures.DepthBuffer));
	geomGenAttachments.Add(FRIFrameBufferAttachment(BufferTextures.NormalBuffer));
	geomGenAttachments.Add(FRIFrameBufferAttachment(BufferTextures.AlbedoBuffer));
	geomGenAttachments.Add(FRIFrameBufferAttachment(BufferTextures.SpecularBuffer));


	GeomGenPipeline.FrameBuffer = cmdList.GetDynamic()->CreateFrameBuffer(geomGenAttachments, true);



	/* Skinned Geometry Gen Pipeline */

	SkinnedGeomGenPipeline.Handle = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["GBufferGenSkinned"]);
}




/****************************************
*			HBAO Stage
*****************************************/



void DeferredRenderer::CreateHBAOStage()
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());

	FArray<FRIFrameBufferAttachment> hbaoAttachment;
	hbaoAttachment.Add(FRIFrameBufferAttachment(BufferTextures.SSAOBuffer));

	HBAOGenPipeline.FrameBuffer = cmdList.GetDynamic()->CreateFrameBuffer(hbaoAttachment, true);


	FHBAOParameters hbaoParams;

	hbaoParams.Radius = 1.0f;
	hbaoParams.Bias = 0.3f;
	hbaoParams.NearAO = 4.0f;
	hbaoParams.FarAO = 1.5f;

	hbaoParams.BackgroundAOEnable = false;
	hbaoParams.BackgroundAO_ViewDepth = 10.0f;

	hbaoParams.ForegroundAOEnable = false;
	hbaoParams.ForegroundAO_ViewDepth = 1.0f;

	hbaoParams.PowerExponent = 2.0f;
	hbaoParams.BlurEnable = true;
	hbaoParams.BlurSharpness = 16.0f;

	HBAOGenPipeline.HbaoService = HBAOPlus::Allocate(renderContext, hbaoParams);
}

/****************************************
*			Combine Stage
*****************************************/



void DeferredRenderer::CreateCombineStage()
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());

	IVector2 viewportSize = renderContext->GetViewport().Size;

	CombinePipeline.Handle = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["Combine"]);


	FScreenQuadUtil::VertexBuffer = cmdList.GetDynamic()->CreateVertexBuffer(
		4, 0, FRICreationDescriptor(
			FScreenQuadUtil::quadVData.Begin(),
			FScreenQuadUtil::quadVData.ByteSize()
		));


	FRIVertexShader* scrQuadSignatureShader = NULL;// cmdList.GetDynamic()->CreateVertexShader(IOFileStream("shaders/signature/dx/bin/CombineQuad.signature.cso").ReadBytes());
	if (renderContext->RenderFramework == EFRIRendererFramework::DX11)
	{
		scrQuadSignatureShader = cmdList.GetDynamic()->CreateVertexShader(IOFileStream("shaders/signature/dx/bin/CombineQuad.signature.cso").ReadBytes());
	}

	FScreenQuadUtil::VertexDeclaration = cmdList.GetDynamic()->CreateVertexDeclaration(FScreenQuadUtil::VertexDeclComp, scrQuadSignatureShader);
	cmdList.GetDynamic()->AttachVertexDeclaration(FScreenQuadUtil::VertexBuffer, FScreenQuadUtil::VertexDeclaration);

	FScreenQuadUtil::IndexBuffer = cmdList.GetDynamic()->CreateIndexBuffer(
		6, 0, FRICreationDescriptor(
			FScreenQuadUtil::quadElementData.Begin(),
			FScreenQuadUtil::quadElementData.ByteSize()
		)
	);


	FScreenQuadUtil::ScreenSpaceMatrix = FOrthographicMatrix(0, viewportSize.x, viewportSize.y, 0, 0.0f, 5.0f);

	blendState = cmdList.GetDynamic()->CreateBlendState(EFRIAlphaBlend::Src, EFRIAlphaBlend::OneMinusSrc);
}

/****************************************
*			Shadowmap Stage
*****************************************/




void DeferredRenderer::CreateShadowmapStage()
{

	FRICommandList cmdList(renderContext->GetFRIDynamic());


	/* Shadowmap Pipeline */
	ShadowmapPipeline.Handle = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["Shadowmap"]);


	ShadowmapPipeline.FrameBuffer = cmdList.GetDynamic()->CreateFrameBuffer(FRIFrameBufferArrayAttachment(BufferTextures.ShadowmapCascadeArray), true);

	/* Shadowmap Skinned Pipeline */
	SkinnedShadowmapPipeline.Handle = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["ShadowmapSkinned"]);
}



void DeferredRenderer::BeginRender(FRICommandList& cmdList)
{

	cmdList.SetShaderUniformBuffer(UBO_SLOT_CAMERA, cameraMatrixBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_TRANSFORM, transformBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_CASCADE_CAMERA, ShadowmapPipeline.CascadeCameraBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_JOINTS, jointBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_COMBINE, CombinePipeline.ConstantBuffer);

	cmdList.SetBlendState(blendState);

}

void DeferredRenderer::Render(FRICommandList& cmdList)
{

	CameraComponent& CameraRef = scene->Camera.Component<CameraComponent>();
	FTransform& CameraPosRef = scene->Camera.Component<FTransform>();
	DirectionalLightComponent& SunRef = scene->Sun.Component<DirectionalLightComponent>();

	// Shadowmap Gen
	cmdList.SetViewport(ShadowmapPipeline.Viewport);

	cmdList.ClearBuffer(ShadowmapPipeline.FrameBuffer, Color::White);
	{
		for (int i = 0; i < 5; i++)
		{
			cmdList.SetFramebufferTextureLayer(ShadowmapPipeline.FrameBuffer, i);
			//cmdList.ClearBuffer(NULL, Color::White);


			// Stage cascade camera buffer
			cmdList.StageResources([&] 
				{
					FRIUpdateDescriptor dataStage(reinterpret_cast<FRIArrayInterface*>(&SunRef.FrustumInfo[i].View), 0, sizeof(CameraComponent));
					cmdList.UniformBufferSubdata(cameraMatrixBuffer, dataStage);

				});


			/*  Static Shadowed Scene  */
			cmdList.SetShaderPipeline(ShadowmapPipeline.Handle);
			SMGeometry->ForEach([&](Entity ent, MeshComponent& mesh, FTransform& transformComponent)
				{
					cmdList.StageResources([&]
						{
							FTransformBufferStruct tr;
							tr.World = transformComponent.GetMatrix();
							tr.WorldInverseTranspose = FMatrix4::Inverse(FMatrix4::Transpose(tr.World));
							FRIUpdateDescriptor dataStage(reinterpret_cast<FRIArrayInterface*>(&tr), 0, sizeof(FTransformBufferStruct));

							cmdList.UniformBufferSubdata(transformBuffer, dataStage);
						});

					cmdList.SetGeometrySource(mesh.VertexBuffer);
					cmdList.DrawPrimitivesIndexed((uint32)EFRIPrimitiveType::Triangles, mesh.IndexBuffer->IndexCount, (uint32)EFRIIndexType::UInt32, mesh.IndexBuffer);

				});


			/*  Skinned Shadowed Scene  */
			cmdList.SetShaderPipeline(SkinnedShadowmapPipeline.Handle);
			SkinnedSMGeometry->ForEach([&](Entity ent, SkinnedMeshComponent& mesh, FTransform& transformComponent)
				{
					cmdList.StageResources([&]
						{
							const FArray<FMatrix4>& jointMatrices = mesh.MeshSkeleton.GetJointTransforms();
							FTransformBufferStruct tr;
							tr.World = transformComponent.GetMatrix();
							tr.WorldInverseTranspose = FMatrix4::Inverse(FMatrix4::Transpose(tr.World));
							FRIUpdateDescriptor dataStage(reinterpret_cast<FRIArrayInterface*>(&tr), 0, sizeof(FTransformBufferStruct));
							FRIUpdateDescriptor jointDataStage(reinterpret_cast<FRIArrayInterface*>(jointMatrices.Begin()), 0, jointMatrices.ByteSize());

							cmdList.UniformBufferSubdata(transformBuffer, dataStage);
							cmdList.UniformBufferSubdata(jointBuffer, jointDataStage);
						});


					cmdList.SetGeometrySource(mesh.VertexBuffer);
					cmdList.DrawPrimitivesIndexed((uint32)EFRIPrimitiveType::Triangles, mesh.IndexBuffer->IndexCount, (uint32)EFRIIndexType::UInt32, mesh.IndexBuffer);

				});
		}


		cmdList.FlushMipMaps(BufferTextures.ShadowmapCascadeArray);
	}
	cmdList.UnbindFrameBuffer();

	cmdList.SetViewport(renderContext->GetViewport());

	// Stage regular camera buffer
	cmdList.StageResources([&]
		{
			FRIUpdateDescriptor camDataStage(reinterpret_cast<FRIArrayInterface*>(&CameraRef), 0, sizeof(CameraComponent));
			cmdList.UniformBufferSubdata(cameraMatrixBuffer, camDataStage);
		});

	// Geom Gen

	cmdList.ClearBuffer(GeomGenPipeline.FrameBuffer, Color::Transparent);
	//cmdList.ClearBuffer(NULL, Color::Transparent);
	{

		/* Static Geometry */

		cmdList.SetShaderPipeline(GeomGenPipeline.Handle);
		Geometry->ForEach([&](Entity ent, MeshComponent& mesh, MaterialComponent& material, FTransform& transformComponent)
			{
				/* Stage DataBuffer */
				cmdList.StageResources([&]
					{
						FTransformBufferStruct tr;
						tr.World = transformComponent.GetMatrix();
						tr.WorldInverseTranspose = FMatrix4::Inverse(FMatrix4::Transpose(tr.World));
						FRIUpdateDescriptor dataStage(reinterpret_cast<FRIArrayInterface*>(&tr), 0, sizeof(FTransformBufferStruct));

						cmdList.UniformBufferSubdata(transformBuffer, dataStage);

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
				cmdList.DrawPrimitivesIndexed((uint32)EFRIPrimitiveType::Triangles, mesh.IndexBuffer->IndexCount, (uint32)EFRIIndexType::UInt32, mesh.IndexBuffer);
			}

		);




		/* Skinned Models */

		cmdList.SetShaderPipeline(SkinnedGeomGenPipeline.Handle);
		SkinnedGeometry->ForEach([&](Entity ent, SkinnedMeshComponent& skinnedMesh, MaterialComponent& material, FTransform& transformComponent)
			{
				/* Stage DataBuffer */
				cmdList.StageResources([&]
					{
						const FArray<FMatrix4>& jointMatrices = skinnedMesh.MeshSkeleton.GetJointTransforms();
						FTransformBufferStruct tr;
						tr.World = transformComponent.GetMatrix();
						tr.WorldInverseTranspose = FMatrix4::Inverse(FMatrix4::Transpose(tr.World));
						FRIUpdateDescriptor dataStage(reinterpret_cast<FRIArrayInterface*>(&tr), 0, sizeof(FTransformBufferStruct));
						FRIUpdateDescriptor jointDataStage(reinterpret_cast<FRIArrayInterface*>(jointMatrices.Begin()), 0, jointMatrices.ByteSize());

						cmdList.UniformBufferSubdata(transformBuffer, dataStage);
						cmdList.UniformBufferSubdata(jointBuffer, jointDataStage);

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

				cmdList.SetGeometrySource(skinnedMesh.VertexBuffer);
				cmdList.DrawPrimitivesIndexed((uint32)EFRIPrimitiveType::Triangles, skinnedMesh.IndexBuffer->IndexCount, (uint32)EFRIIndexType::UInt32, skinnedMesh.IndexBuffer);
			}

		);
	}
	cmdList.UnbindFrameBuffer();

	HBAOGenPipeline.HbaoService->RenderAO(BufferTextures.DepthBuffer, BufferTextures.NormalBuffer, HBAOGenPipeline.FrameBuffer, CameraRef.Projection, CameraRef.View);

	cmdList.ClearBuffer(NULL, Color::DeepSkyBlue);
	{
		cmdList.SetShaderPipeline(CombinePipeline.Handle);
		cmdList.SetShaderSampler(FUniformSampler(0, BufferTextures.DepthBuffer));
		cmdList.SetShaderSampler(FUniformSampler(1, BufferTextures.NormalBuffer));
		cmdList.SetShaderSampler(FUniformSampler(2, BufferTextures.AlbedoBuffer));
		cmdList.SetShaderSampler(FUniformSampler(3, BufferTextures.SpecularBuffer));
		cmdList.SetShaderSampler(FUniformSampler(4, BufferTextures.ShadowmapCascadeArray));
		cmdList.SetShaderSampler(FUniformSampler(5, BufferTextures.SSAOBuffer));


		cmdList.StageResources([&]
			{
				FCombineBufferStruct buffer;
				buffer.Direction.rgb = SunRef.Direction;
				buffer.Info = FVector4(SunRef.LightColor.rgb, SunRef.Intensity);
				buffer.InverseView = FMatrix4::Inverse(CameraRef.View);
				buffer.InverseProjection = FMatrix4::Inverse(CameraRef.Projection);

				FRIUpdateDescriptor dataStage(&buffer, 0, sizeof(FCombineBufferStruct));
				FRIUpdateDescriptor cascadeStage(reinterpret_cast<FRIArrayInterface*>(SunRef.FrustumInfo.Begin()), 0, sizeof(FViewFrustumInfo) * 5);

				cmdList.UniformBufferSubdata(CombinePipeline.ConstantBuffer, dataStage);
				cmdList.UniformBufferSubdata(ShadowmapPipeline.CascadeCameraBuffer, cascadeStage);

			});

		DrawScreenQuad(cmdList);
	}


}
void DeferredRenderer::EndRender(FRICommandList& cmdList)
{


}
void DeferredRenderer::GBufferGen(FRICommandList& cmdList)
{

}

void DeferredRenderer::RenderCombine(FRICommandList& cmdList)
{

}
void DeferredRenderer::RenderAO(FRICommandList& cmdList)
{

}


void DeferredRenderer::DrawScreenQuad(FRICommandList& cmdList)
{
	cmdList.SetGeometrySource(FScreenQuadUtil::VertexBuffer);
	cmdList.DrawPrimitivesIndexed((uint32)EFRIPrimitiveType::Triangles, 6, (uint32)EFRIIndexType::UInt32, FScreenQuadUtil::IndexBuffer);
}


void DeferredRenderer::AttachToScene(Scene* scene)
{
	this->scene = scene;

	Geometry = scene->RegisterSystem<FRenderSystemGeom>(ECSExecutionFlag::USER_TICK);
	SkinnedGeometry = scene->RegisterSystem<FRenderSystemSkinnedGeom>(ECSExecutionFlag::USER_TICK);


	SMGeometry = scene->RegisterSystem<FRenderSystemSM>(ECSExecutionFlag::USER_TICK);
	SkinnedSMGeometry = scene->RegisterSystem<FRenderSystemSkinnedSM>(ECSExecutionFlag::USER_TICK);


}