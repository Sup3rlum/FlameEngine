#include "DeferredRenderer.h"


#include "Core/Framework/IO/FileStream.h"
#include "../ContentSystem/Client/AssetImportScripts/Material.h"



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



void DeferredRenderer::CreateResources(FRIContext* renderContext)
{

	FRICommandList cmdList(renderContext->GetFRIDynamic());


	FResourceTextureColorDescriptor depthDescriptor(EFRITextureChannelStorage::RG32F, EFRITextureChannels::RGBA, EFRITexturePixelStorage::Float);
	FResourceTextureColorDescriptor bufferDescriptor(EFRITextureChannelStorage::RGBA32F, EFRITextureChannels::RGBA, EFRITexturePixelStorage::Float);
	FResourceTextureColorDescriptor shadowDescriptor(EFRITextureChannelStorage::RG32F, EFRITextureChannels::RGBA, EFRITexturePixelStorage::Float);


	FArray<FTextureParameterBufferParameter> paramBuffer1;

	paramBuffer1.Add(FTextureParameterBufferParameter(EFRITextureParamName::MinFilter, EFRITextureFilterMode::Trilinear));
	paramBuffer1.Add(FTextureParameterBufferParameter(EFRITextureParamName::MagFilter, EFRITextureFilterMode::Trilinear));
	paramBuffer1.Add(FTextureParameterBufferParameter(EFRITextureParamName::AnisotropyLevel, 16.0f));



	/* Texture Buffer Gen */

	IVector2 viewportSize = renderContext->GetViewportSize();

	BufferTextures.DepthBuffer		= cmdList.GetDynamic()->DynamicCreateTexture2D(viewportSize.x, viewportSize.y, 0, depthDescriptor);
	BufferTextures.AlbedoBuffer		= cmdList.GetDynamic()->DynamicCreateTexture2D(viewportSize.x, viewportSize.y, 0, bufferDescriptor);
	BufferTextures.NormalBuffer		= cmdList.GetDynamic()->DynamicCreateTexture2D(viewportSize.x, viewportSize.y, 0, bufferDescriptor);
	BufferTextures.SpecularBuffer	= cmdList.GetDynamic()->DynamicCreateTexture2D(viewportSize.x, viewportSize.y, 0, bufferDescriptor);
	BufferTextures.SSAOBuffer		= cmdList.GetDynamic()->DynamicCreateTexture2D(viewportSize.x, viewportSize.y, 0, bufferDescriptor);


	BufferTextures.ShadowmapCascadeArray = cmdList.GetDynamic()->DynamicCreateTexture2DArray(4096, 4096, 5, shadowDescriptor);


	cmdList.SetTextureParameterBuffer(BufferTextures.DepthBuffer, FResourceTextureParameterBuffer(paramBuffer1));
	cmdList.SetTextureParameterBuffer(BufferTextures.AlbedoBuffer, FResourceTextureParameterBuffer(paramBuffer1));
	cmdList.SetTextureParameterBuffer(BufferTextures.NormalBuffer, FResourceTextureParameterBuffer(paramBuffer1));
	cmdList.SetTextureParameterBuffer(BufferTextures.SpecularBuffer, FResourceTextureParameterBuffer(paramBuffer1));
	cmdList.SetTextureParameterBuffer(BufferTextures.SSAOBuffer, FResourceTextureParameterBuffer(paramBuffer1));
	cmdList.SetTextureParameterBuffer(BufferTextures.ShadowmapCascadeArray, FResourceTextureParameterBuffer(paramBuffer1));


	/* -------------------------------- Geometry Gen Pipeline */
	


	FAnsiString vsData = IOFileStream("shaders/deferred/GBufferGen.vert").ReadAnsiFile();
	FAnsiString psData = IOFileStream("shaders/deferred/GBufferGen.frag").ReadAnsiFile();

	FResourceVertexShader* vs = cmdList.GetDynamic()->DynamicCreateVertexShader(vsData);
	FResourcePixelShader* ps = cmdList.GetDynamic()->DynamicCreatePixelShader(psData);


	GeomGenPipeline.Handle = cmdList.GetDynamic()->DynamicCreateShaderPipeline
	(
		FResourceShaderPipelineCreationDescriptor(
			2, 
			new FResourceShaderBase* [2]{ vs, ps }
		)
	);

	GeomGenPipeline.ViewLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(GeomGenPipeline.Handle, "View");
	GeomGenPipeline.ProjLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(GeomGenPipeline.Handle, "Projection");
	GeomGenPipeline.WorldLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(GeomGenPipeline.Handle, "World");

	FArray<FResourceFrameBufferTextureAttachment> geomGenAttachments;
	geomGenAttachments.Add(FResourceFrameBufferTextureAttachment(BufferTextures.DepthBuffer, EResourceFBTextureAttachmentType::Color));
	geomGenAttachments.Add(FResourceFrameBufferTextureAttachment(BufferTextures.NormalBuffer, EResourceFBTextureAttachmentType::Color));
	geomGenAttachments.Add(FResourceFrameBufferTextureAttachment(BufferTextures.AlbedoBuffer, EResourceFBTextureAttachmentType::Color));
	geomGenAttachments.Add(FResourceFrameBufferTextureAttachment(BufferTextures.SpecularBuffer, EResourceFBTextureAttachmentType::Color));


	GeomGenPipeline.FrameBuffer = cmdList.GetDynamic()->DynamicCreateFrameBuffer(geomGenAttachments, true);


	/* -------------------------------- Skinned Geometry Gen Pipeline */


	vsData = IOFileStream("shaders/deferred/GBufferGenSkinned.vert").ReadAnsiFile();
	psData = IOFileStream("shaders/deferred/GBufferGen.frag").ReadAnsiFile();

	vs = cmdList.GetDynamic()->DynamicCreateVertexShader(vsData);
	ps = cmdList.GetDynamic()->DynamicCreatePixelShader(psData);


	SkinnedGeomGenPipeline.Handle = cmdList.GetDynamic()->DynamicCreateShaderPipeline
	(
		FResourceShaderPipelineCreationDescriptor(
			2,
			new FResourceShaderBase * [2]{ vs, ps }
		)
	);

	SkinnedGeomGenPipeline.ViewLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(SkinnedGeomGenPipeline.Handle, "View");
	SkinnedGeomGenPipeline.ProjLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(SkinnedGeomGenPipeline.Handle, "Projection");
	SkinnedGeomGenPipeline.WorldLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(SkinnedGeomGenPipeline.Handle, "World");
	SkinnedGeomGenPipeline.JointBufferLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(SkinnedGeomGenPipeline.Handle, "jointTransforms");


	/* -------------------------------- HBAO Pipeline */

	FArray<FResourceFrameBufferTextureAttachment> hbaoAttachment;
	hbaoAttachment.Add(FResourceFrameBufferTextureAttachment(BufferTextures.SSAOBuffer, EResourceFBTextureAttachmentType::Color));

	HBAOGenPipeline.FrameBuffer = cmdList.GetDynamic()->DynamicCreateFrameBuffer(hbaoAttachment, true);


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


	/* Shadowmap Pipeline */



	vsData = IOFileStream("shaders/deferred/Shadowmap.vert").ReadAnsiFile();
	psData = IOFileStream("shaders/deferred/Shadowmap.frag").ReadAnsiFile();

	vs = cmdList.GetDynamic()->DynamicCreateVertexShader(vsData);
	ps = cmdList.GetDynamic()->DynamicCreatePixelShader(psData);

	ShadowmapPipeline.Handle = cmdList.GetDynamic()->DynamicCreateShaderPipeline
	(
		FResourceShaderPipelineCreationDescriptor(
			2,
			new FResourceShaderBase * [2]{ vs, ps }
		)
	);


	FArray<FResourceFrameBufferTextureAttachment> shadowmapAttachment;
	shadowmapAttachment.Add(FResourceFrameBufferTextureAttachment(BufferTextures.ShadowmapCascadeArray, EResourceFBTextureAttachmentType::Color));
	ShadowmapPipeline.FrameBuffer = cmdList.GetDynamic()->DynamicCreateFrameBuffer(shadowmapAttachment, true);

	ShadowmapPipeline.ViewLoc =  cmdList.GetDynamic()->GetShaderUniformParameterLocation(ShadowmapPipeline.Handle, "View");
	ShadowmapPipeline.ProjLoc =  cmdList.GetDynamic()->GetShaderUniformParameterLocation(ShadowmapPipeline.Handle, "Projection");
	ShadowmapPipeline.WorldLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(ShadowmapPipeline.Handle, "World");


	/* Combine Pipeline */

	vsData = IOFileStream("shaders/deferred/Combine.vert").ReadAnsiFile();
	psData = IOFileStream("shaders/deferred/Combine.frag").ReadAnsiFile();

	vs = cmdList.GetDynamic()->DynamicCreateVertexShader(vsData);
	ps = cmdList.GetDynamic()->DynamicCreatePixelShader(psData);

	CombinePipeline.Handle = cmdList.GetDynamic()->DynamicCreateShaderPipeline
	(
		FResourceShaderPipelineCreationDescriptor(
			2,
			new FResourceShaderBase * [2]{ vs, ps }
		)
	);


	FScreenQuadUtil::VertexBuffer = cmdList.GetDynamic()->DynamicCreateVertexBuffer(
		4, 0, FResourceCreationDescriptor(
			FScreenQuadUtil::quadVData.Begin(),
			FScreenQuadUtil::quadVData.ByteSize()
		));
	

	cmdList.GetDynamic()->AttachVertexDeclaration(FScreenQuadUtil::VertexDecl);

	FScreenQuadUtil::IndexBuffer = cmdList.GetDynamic()->DynamicCreateIndexBuffer(
		6, 0, FResourceCreationDescriptor(
			FScreenQuadUtil::quadElementData.Begin(),
			FScreenQuadUtil::quadElementData.ByteSize()
		)
	);


	FScreenQuadUtil::ScreenSpaceMatrix = FOrthographicMatrix(0, viewportSize.x, viewportSize.y, 0, 0.0f, 5.0f);


	CombinePipeline.DirectionalLightsDirection = cmdList.GetDynamic()->GetShaderUniformParameterLocation(CombinePipeline.Handle, "DirectionalLights[0].Direction");
	CombinePipeline.DirectionalLightsColor = cmdList.GetDynamic()->GetShaderUniformParameterLocation(CombinePipeline.Handle, "DirectionalLights[0].Color");
	CombinePipeline.DirectionalLightsIntensity = cmdList.GetDynamic()->GetShaderUniformParameterLocation(CombinePipeline.Handle, "DirectionalLights[0].Intensity");



	FAnsiString name1[5] =
	{
		"DirectionalLights[0].Info[0].VPMatrix",
		"DirectionalLights[0].Info[1].VPMatrix",
		"DirectionalLights[0].Info[2].VPMatrix",
		"DirectionalLights[0].Info[3].VPMatrix",
		"DirectionalLights[0].Info[4].VPMatrix"
	};


	FAnsiString name2[5] =
	{
		"DirectionalLights[0].Info[0].Depth",
		"DirectionalLights[0].Info[1].Depth",
		"DirectionalLights[0].Info[2].Depth",
		"DirectionalLights[0].Info[3].Depth",
		"DirectionalLights[0].Info[4].Depth"
	};

	FAnsiString name3[5] =
	{
		"TestColors[0]",
		"TestColors[1]",
		"TestColors[2]",
		"TestColors[3]",
		"TestColors[4]"
	};

	for (int i = 0; i < 5; i++)
	{
		CombinePipeline.DirectionalLightsVPMatrix[i] = cmdList.GetDynamic()->GetShaderUniformParameterLocation(CombinePipeline.Handle, name1[i]);
		CombinePipeline.DirectionalLightsDepth[i] = cmdList.GetDynamic()->GetShaderUniformParameterLocation(CombinePipeline.Handle, name2[i]);
		CombinePipeline.ColorsLoc[i] = cmdList.GetDynamic()->GetShaderUniformParameterLocation(CombinePipeline.Handle, name3[i]);
	}

	CombinePipeline.InverseView = cmdList.GetDynamic()->GetShaderUniformParameterLocation(CombinePipeline.Handle, "InverseView");
	CombinePipeline.InverseProj = cmdList.GetDynamic()->GetShaderUniformParameterLocation(CombinePipeline.Handle, "InverseProjection");


	CombinePipeline.PointPosition = cmdList.GetDynamic()->GetShaderUniformParameterLocation(CombinePipeline.Handle, "PointLights[0].Position");
	CombinePipeline.PointIntensity = cmdList.GetDynamic()->GetShaderUniformParameterLocation(CombinePipeline.Handle, "PointLights[0].Intensity");
	CombinePipeline.PointRadius = cmdList.GetDynamic()->GetShaderUniformParameterLocation(CombinePipeline.Handle, "PointLights[0].Radius");
	CombinePipeline.PointColor = cmdList.GetDynamic()->GetShaderUniformParameterLocation(CombinePipeline.Handle, "PointLights[0].Color");

	CombinePipeline.CamPos = cmdList.GetDynamic()->GetShaderUniformParameterLocation(CombinePipeline.Handle, "CameraPosition");
	CombinePipeline.View = cmdList.GetDynamic()->GetShaderUniformParameterLocation(CombinePipeline.Handle, "View");



	/* QUAD PIPELINE */


	vsData = IOFileStream("shaders/basic/quad.vert").ReadAnsiFile();
	psData = IOFileStream("shaders/basic/quad.frag").ReadAnsiFile();

	vs = cmdList.GetDynamic()->DynamicCreateVertexShader(vsData);
	ps = cmdList.GetDynamic()->DynamicCreatePixelShader(psData);

	quadDrawPipeline = cmdList.GetDynamic()->DynamicCreateShaderPipeline
	(
		FResourceShaderPipelineCreationDescriptor(
			2,
			new FResourceShaderBase * [2]{ vs, ps }
		)
	);

	TransformLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(quadDrawPipeline, "Transform");
	ProjLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(quadDrawPipeline, "Projection");


	/* QUAD ARRAY */

	vsData = IOFileStream("shaders/basic/quad_array.vert").ReadAnsiFile();
	psData = IOFileStream("shaders/basic/quad_array.frag").ReadAnsiFile();

	vs = cmdList.GetDynamic()->DynamicCreateVertexShader(vsData);
	ps = cmdList.GetDynamic()->DynamicCreatePixelShader(psData);

	quadDrawArrayPipeline = cmdList.GetDynamic()->DynamicCreateShaderPipeline
	(
		FResourceShaderPipelineCreationDescriptor(
			2,
			new FResourceShaderBase * [2]{ vs, ps }
		)
	);

	TransformLocArray = cmdList.GetDynamic()->GetShaderUniformParameterLocation(quadDrawArrayPipeline, "Transform");
	ProjLocArray = cmdList.GetDynamic()->GetShaderUniformParameterLocation(quadDrawArrayPipeline, "Projection");
	LayerLocArray = cmdList.GetDynamic()->GetShaderUniformParameterLocation(quadDrawArrayPipeline, "Layer");
	MaxLayerLocArray = cmdList.GetDynamic()->GetShaderUniformParameterLocation(quadDrawArrayPipeline, "MaxLayers");



	/* Box volume debug */
	serv = new BoundingVolumeDebugService(cmdList);

	atmosphereRenderer.CreateResources(renderContext);
	satGenerator.CreateResources(renderContext);
	satGenerator.SetupContext(FVector2(4096, 4096), 5);


	for (int i = 0; i < 26; i++)
	{
		matrices.Add(FMatrix4(1));
	}
}



void DeferredRenderer::BeginRender(FRICommandList& cmdList, Scene* sceneToRender)
{

	CameraComponent& camRef = sceneToRender->Camera.Component<CameraComponent>();
	TransformComponent& tRef = sceneToRender->Camera.Component<TransformComponent>();


	auto meshSystem = sceneToRender->CreateSystem<MeshComponent, MaterialComponent, TransformComponent>();
	auto skinnedMeshSystem = sceneToRender->CreateSystem<SkinnedMeshComponent, MaterialComponent, TransformComponent>();

	auto shadowmapSystem = sceneToRender->CreateSystem<MeshComponent, TransformComponent>();


	DirectionalLightComponent& dirLightRef = sceneToRender->Sun.Component<DirectionalLightComponent>();

	// Shadowmap Gen
	
	
	cmdList.SetViewport(0, 0, 4096, 4096);

	cmdList.ClearBuffer(ShadowmapPipeline.FrameBuffer, Color::White);
	{
		for (int i = 0; i < 5; i++)
		{
			cmdList.SetFramebufferTextureLayer(BufferTextures.ShadowmapCascadeArray, i);

			cmdList.ClearBuffer(NULL, Color::White);

			cmdList.SetShaderPipeline(ShadowmapPipeline.Handle);
			cmdList.SetShaderUniformParameter(FUniformParameter(ShadowmapPipeline.ViewLoc, dirLightRef.FrustumInfo[i].View));
			cmdList.SetShaderUniformParameter(FUniformParameter(ShadowmapPipeline.ProjLoc, dirLightRef.FrustumInfo[i].Projection));


			shadowmapSystem.ForEach([&](Entity ent, MeshComponent& mesh, TransformComponent& transformComponent)
				{
					cmdList.SetShaderUniformParameter(FUniformParameter(ShadowmapPipeline.WorldLoc, transformComponent.Transform.GetMatrix()));

					cmdList.SetGeometrySource(mesh.VertexBuffer);
					cmdList.DrawPrimitivesIndexed((uint32)EFRIPrimitiveType::Triangles, mesh.IndexBuffer->IndexCount, (uint32)EFRIIndexType::UInt32, mesh.IndexBuffer);

				});
		}


		cmdList.FlushMipMaps(BufferTextures.ShadowmapCascadeArray);
	}
	cmdList.UnbindFrameBuffer();

	//satGenerator.Render(cmdList, BufferTextures.ShadowmapCascadeArray, ShadowmapPipeline.FrameBuffer);

	cmdList.SetViewport(0, 0, 2560, 1440);

	// Geom Gen

	cmdList.ClearBuffer(GeomGenPipeline.FrameBuffer, Color::Transparent);
	{

		// REGULAR

		cmdList.SetShaderPipeline(GeomGenPipeline.Handle);
		cmdList.SetShaderUniformParameter(FUniformParameter(GeomGenPipeline.ViewLoc, camRef.View));
		cmdList.SetShaderUniformParameter(FUniformParameter(GeomGenPipeline.ProjLoc, camRef.Projection));


		meshSystem.ForEach([&](Entity ent, MeshComponent& mesh, MaterialComponent& material, TransformComponent& transformComponent)
			{
				cmdList.SetShaderUniformParameter(FUniformParameter(GeomGenPipeline.WorldLoc, transformComponent.Transform.GetMatrix()));

				for (int map = 0; map < (uint32)EMaterialMap::MAX_MAPS; map++)
				{
					FResourceTexture2D* textureMap = material.GetMap((EMaterialMap)map).Handle;

					if (textureMap)
					{
						cmdList.SetShaderSampler(FUniformSampler(map, textureMap));
					}
				}

				cmdList.SetGeometrySource(mesh.VertexBuffer);
				cmdList.DrawPrimitivesIndexed((uint32)EFRIPrimitiveType::Triangles, mesh.IndexBuffer->IndexCount, (uint32)EFRIIndexType::UInt32, mesh.IndexBuffer);
			}

		);

		// SKINNED

		cmdList.SetShaderPipeline(SkinnedGeomGenPipeline.Handle);
		cmdList.SetShaderUniformParameter(FUniformParameter(SkinnedGeomGenPipeline.ViewLoc, camRef.View));
		cmdList.SetShaderUniformParameter(FUniformParameter(SkinnedGeomGenPipeline.ProjLoc, camRef.Projection));


		skinnedMeshSystem.ForEach([&](Entity ent, SkinnedMeshComponent& skinnedMesh, MaterialComponent& material, TransformComponent& transformComponent)
			{
				cmdList.SetShaderUniformParameter(FUniformParameter(SkinnedGeomGenPipeline.WorldLoc, transformComponent.Transform.GetMatrix()));
				cmdList.SetShaderUniformParameter(FUniformParameter(SkinnedGeomGenPipeline.JointBufferLoc, skinnedMesh.Skeleton.GetJointTransforms()));
				//cmdList.SetShaderUniformParameter(FUniformParameter(SkinnedGeomGenPipeline.JointBufferLoc, matrices));

				for (int map = 0; map < (uint32)EMaterialMap::MAX_MAPS; map++)
				{
					FResourceTexture2D* textureMap = material.GetMap((EMaterialMap)map).Handle;

					if (textureMap)
					{
						cmdList.SetShaderSampler(FUniformSampler(map, textureMap));
					}
				}

				cmdList.SetGeometrySource(skinnedMesh.VertexBuffer);
				cmdList.DrawPrimitivesIndexed((uint32)EFRIPrimitiveType::Triangles, skinnedMesh.IndexBuffer->IndexCount, (uint32)EFRIIndexType::UInt32, skinnedMesh.IndexBuffer);
			}

		);





		cmdList.FlushMipMaps(BufferTextures.DepthBuffer);
		cmdList.FlushMipMaps(BufferTextures.AlbedoBuffer);
		cmdList.FlushMipMaps(BufferTextures.NormalBuffer);
	}
	cmdList.UnbindFrameBuffer();


	HBAOGenPipeline.HbaoService->RenderAO(BufferTextures.DepthBuffer, BufferTextures.NormalBuffer, HBAOGenPipeline.FrameBuffer, camRef.Projection, camRef.View);

	

	cmdList.ClearBuffer(NULL, Color::DeepSkyBlue);
	{

		atmosphereRenderer.RenderSkySphere(cmdList, camRef, tRef);


		cmdList.SetShaderPipeline(CombinePipeline.Handle);
		cmdList.SetShaderSampler(FUniformSampler(0, BufferTextures.DepthBuffer));
		cmdList.SetShaderSampler(FUniformSampler(1, BufferTextures.NormalBuffer));
		cmdList.SetShaderSampler(FUniformSampler(2, BufferTextures.AlbedoBuffer));
		cmdList.SetShaderSampler(FUniformSampler(3, BufferTextures.SpecularBuffer));
		cmdList.SetShaderSampler(FUniformSampler(4, BufferTextures.ShadowmapCascadeArray));
		cmdList.SetShaderSampler(FUniformSampler(5, BufferTextures.SSAOBuffer));


		cmdList.SetShaderUniformParameter(FUniformParameter(CombinePipeline.DirectionalLightsDirection, FVector3::Normalize(FVector3(-1, -1, -1))));
		cmdList.SetShaderUniformParameter(FUniformParameter(CombinePipeline.DirectionalLightsColor, Color::CosmicLatte));
		cmdList.SetShaderUniformParameter(FUniformParameter(CombinePipeline.DirectionalLightsIntensity, 1.0f));

		for (int i = 0; i < 5; i++)
		{
			cmdList.SetShaderUniformParameter(FUniformParameter(CombinePipeline.DirectionalLightsVPMatrix[i], dirLightRef.FrustumInfo[i].Projection * dirLightRef.FrustumInfo[i].View));
			cmdList.SetShaderUniformParameter(FUniformParameter(CombinePipeline.DirectionalLightsDepth[i], dirLightRef.FrustumInfo[i].Depth));

		}

		cmdList.SetShaderUniformParameter(FUniformParameter(CombinePipeline.View, camRef.View));
		cmdList.SetShaderUniformParameter(FUniformParameter(CombinePipeline.InverseView, FMatrix4::Inverse(camRef.View)));
		cmdList.SetShaderUniformParameter(FUniformParameter(CombinePipeline.InverseProj, FMatrix4::Inverse(camRef.Projection)));

		cmdList.SetShaderUniformParameter(FUniformParameter(CombinePipeline.CamPos, tRef.Transform.Position));

		/*
		cmdList.SetShaderUniformParameter(FUniformParameter(CombinePipeline.PointPosition, FVector3(0, 3, 0)));
		cmdList.SetShaderUniformParameter(FUniformParameter(CombinePipeline.PointColor, Color::Red));
		cmdList.SetShaderUniformParameter(FUniformParameter(CombinePipeline.PointRadius, 10.0f));
		cmdList.SetShaderUniformParameter(FUniformParameter(CombinePipeline.PointIntensity, 2.0f));*/

		DrawScreenQuad(cmdList);

	}

	/*DrawDebugQuad(cmdList, FVector2(1000, 0), FVector2(200, 200), BufferTextures.DepthBuffer);
	DrawDebugQuad(cmdList, FVector2(1200, 0), FVector2(200, 200), BufferTextures.NormalBuffer);
	DrawDebugQuad(cmdList, FVector2(1400, 0), FVector2(200, 200), BufferTextures.AlbedoBuffer);*/


	
	/*DrawDebugQuadLayer(cmdList, FVector2(1400, 0), FVector2(200, 200), BufferTextures.ShadowmapCascadeArray, 0);
	DrawDebugQuadLayer(cmdList, FVector2(1600, 0), FVector2(200, 200), BufferTextures.ShadowmapCascadeArray, 1);
	DrawDebugQuadLayer(cmdList, FVector2(1800, 0), FVector2(200, 200), BufferTextures.ShadowmapCascadeArray, 2);
	DrawDebugQuadLayer(cmdList, FVector2(2000, 0), FVector2(200, 200), BufferTextures.ShadowmapCascadeArray, 3);
	DrawDebugQuadLayer(cmdList, FVector2(2200, 0), FVector2(200, 200), BufferTextures.ShadowmapCascadeArray, 4);*/



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

void DeferredRenderer::DrawDebugQuad(FRICommandList& cmdList, FVector2 pos, FVector2 size, FResourceTexture2D* tex)
{

	FMatrix4 transform = FTranslationMatrix(FVector3(pos, 0)) * FScalingMatrix(FVector3(size.x, size.y, 1)) * FTranslationMatrix(FVector3(1,1,0)) * FScalingMatrix(FVector3(0.5f, -0.5f, 1.0f));

	cmdList.SetShaderPipeline(quadDrawPipeline);
	cmdList.SetShaderSampler(FUniformSampler(0, tex));
	cmdList.SetShaderUniformParameter(FUniformParameter(ProjLoc, FScreenQuadUtil::ScreenSpaceMatrix));
	cmdList.SetShaderUniformParameter(FUniformParameter(TransformLoc, transform));
	DrawScreenQuad(cmdList);
}


void DeferredRenderer::DrawDebugQuadLayer(FRICommandList& cmdList, FVector2 pos, FVector2 size, FResourceTexture2DArray* tex, uint32 layer)
{

	FMatrix4 transform = FTranslationMatrix(FVector3(pos, 0)) * FScalingMatrix(FVector3(size.x, size.y, 1)) * FTranslationMatrix(FVector3(1, 1, 0)) * FScalingMatrix(FVector3(0.5f, -0.5f, 1.0f));

	cmdList.SetShaderPipeline(quadDrawArrayPipeline);
	cmdList.SetShaderSampler(FUniformSampler(0, tex));
	cmdList.SetShaderUniformParameter(FUniformParameter(ProjLocArray, FScreenQuadUtil::ScreenSpaceMatrix));
	cmdList.SetShaderUniformParameter(FUniformParameter(TransformLocArray, transform));
	cmdList.SetShaderUniformParameter(FUniformParameter(LayerLocArray, float(layer)));
	cmdList.SetShaderUniformParameter(FUniformParameter(MaxLayerLocArray, 5.0f));
	DrawScreenQuad(cmdList);
}
