#include "DeferredRenderer.h"


#include "Core/Framework/IO/FileStream.h"
#include "../ContentSystem/Client/AssetImportScripts/Material.h"

#include "Deferred/BRDF.h"

#define UBO_SLOT_CAMERA 0
#define UBO_SLOT_TRANSFORM 1
#define UBO_SLOT_CASCADE 2
#define UBO_SLOT_JOINTS 3
#define UBO_SLOT_LIGHTING 4
#define UBO_SLOT_MATERIAL 5
#define UBO_SLOT_DLIGHT_DATA 6
#define UBO_SLOT_PLIGHT_DATA 7
#define UBO_SLOT_SLIGHT_DATA 8
#define UBO_SLOT_VXGI 10



#define LI_SLOT_DEPTH 0	
#define LI_SLOT_NORMAL 1	
#define LI_SLOT_ALBEDO 2	
#define LI_SLOT_MR 3
#define LI_SLOT_EMISSIVE 4
#define LI_SLOT_AO 5
#define LI_SLOT_SHADOWMAP 6

#define LI_SLOT_BRDF 7
#define LI_SLOT_TRANSLUSCENCY 8
#define LI_SLOT_VX_RADIANCE 9
#define LI_SLOT_VX_ANISO 10


void DeferredRenderer::_DRUBuffers::Create(FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());

	CameraMatrix	= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(CameraComponent)));
	Transform		= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FMatrix4) * 2));
	JointData		= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FMatrix4) * 32));
	Material		= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, Material::GetStageMemorySize()));

	CascadeData = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FViewFrustumInfo) * SM_CASCADES));

	DLight = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, DirectionalLight::GetStageMemorySize() * 32));
	PLight = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, PointLight::GetStageMemorySize() * 32));
	SLight = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, SpotLight::GetStageMemorySize() * 32));

	LightingConstants = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, 76 * sizeof(float)));
}


void DeferredRenderer::_DRFBO::Create(FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());
	IVector2 viewportSize = renderContext->GetViewport().Size;

	Albedo				= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);
	Normal				= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);
	MetallicRoughness	= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);
	Emissive			= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);

	PostProcessTex		= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);
	LightingTex			= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA32F);

	auto ShadowmapViewport = FViewportRect(0, 0, 4096, 4096);
	ShadowmapArray = cmdList.GetDynamic()->CreateTexture2DArray(ShadowmapViewport.Width, ShadowmapViewport.Height, SM_CASCADES, EFRITextureFormat::RG32F, FRIColorDataFormat(EFRIChannels::RG, EFRIPixelStorage::Float));




	FArray<FRIFrameBufferAttachment> geomGenAttachments;
	//geomGenAttachments.Add(FRIFrameBufferAttachment(Depth));
	geomGenAttachments.Add(FRIFrameBufferAttachment(Normal));
	geomGenAttachments.Add(FRIFrameBufferAttachment(Albedo));
	geomGenAttachments.Add(FRIFrameBufferAttachment(MetallicRoughness));
	geomGenAttachments.Add(FRIFrameBufferAttachment(Emissive));
	GBuffer		= cmdList.GetDynamic()->CreateFrameBuffer(geomGenAttachments, true);
	SMBuffer	= cmdList.GetDynamic()->CreateFrameBuffer(FRIFrameBufferArrayAttachment(ShadowmapArray), true);
	PostProcess = cmdList.GetDynamic()->CreateFrameBuffer({ FRIFrameBufferAttachment(PostProcessTex) }, true);
	Lighting	= cmdList.GetDynamic()->CreateFrameBuffer({ FRIFrameBufferAttachment(LightingTex) }, false);


	BRDFLut = cmdList.GetDynamic()->CreateTexture2D(
		BRDF_LUT_WIDTH,
		BRDF_LUT_HEIGHT,
		0,
		EFRITextureFormat::RG8UNORM,
		FRIColorDataFormat(EFRIChannels::RG, EFRIPixelStorage::UnsignedByte),
		FRICreationDescriptor(brdfTex, BRDF_LUT_SIZE));
}


void DeferredRenderer::_DRShaders::Create(FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());

	ShaderLibrary deferredLib = FLocalContent::LoadFromLocal<ShaderLibrary>("shaders/deferred_dx.fslib", renderContext);

	GShader			= cmdList.GetDynamic()->CreateShaderPipeline(deferredLib.Modules["GBufferGen"]);
	GSkinnedShader	= cmdList.GetDynamic()->CreateShaderPipeline(deferredLib.Modules["GBufferGenSkinned"]);
	Lighting		= cmdList.GetDynamic()->CreateShaderPipeline(deferredLib.Modules["Lighting"]);
	PostProcess		= cmdList.GetDynamic()->CreateShaderPipeline(deferredLib.Modules["PostProcess"]);
	SMShader		= cmdList.GetDynamic()->CreateShaderPipeline(deferredLib.Modules["Shadowmap"]);
	SMSkinnedShader	= cmdList.GetDynamic()->CreateShaderPipeline(deferredLib.Modules["ShadowmapSkinned"]);
}

void DeferredRenderer::CreateResources(FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());
	Viewport = renderContext->GetViewport();
	ShadowmapViewport = FViewportRect(0, 0, 4096, 4096);

	DefaultBlend = cmdList.GetDynamic()->CreateBlendState(EFRIAlphaBlend::Src, EFRIAlphaBlend::OneMinusSrc);
	DisableDepth = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);
	DefaultDepth = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::True, EFRIBool::False);
	DefaultRasterizer = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);

	CreateRenderUtil(renderContext);

	Shaders.Create(renderContext);
	UBuffers.Create(renderContext);
	FrameBuffers.Create(renderContext);

	atmosphereRenderer.CreateResources(renderContext);
	debugRenderer.CreateResources(renderContext);
	vxgiRenderer.CreateResources(renderContext);
	smaa.CreateResources(renderContext);


	this->RenderModules.Add(&atmosphereRenderer);
	this->RenderModules.Add(&debugRenderer);
	this->RenderModules.Add(&vxgiRenderer);
	this->RenderModules.Add(&smaa);
}

void DeferredRenderer::RecreateResources(FRIContext* renderContext, FRIContext* prevContext)
{

}


void DeferredRenderer::CreateRenderUtil(FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());

	/* Create Render Stage resources */

	FRenderUtil::VertexBuffer = cmdList.GetDynamic()->CreateVertexBuffer(
		4, 0, FRICreationDescriptor(
			FRenderUtil::quadVData.Begin(),
			FRenderUtil::quadVData.ByteSize()
		));


	FRIVertexShader* scrQuadSignatureShader = NULL;
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
}


void DeferredRenderer::BeginRender(FRICommandList& cmdList)
{



}

void DeferredRenderer::Render(FRICommandList& cmdList)
{

	cmdList.SetShaderUniformBuffer(UBO_SLOT_CAMERA,		UBuffers.CameraMatrix);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_TRANSFORM,	UBuffers.Transform);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_JOINTS,		UBuffers.JointData);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_MATERIAL,	UBuffers.Material);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_CASCADE,	UBuffers.CascadeData);

	cmdList.SetShaderUniformBuffer(UBO_SLOT_LIGHTING,	UBuffers.LightingConstants);

	cmdList.SetShaderUniformBuffer(UBO_SLOT_DLIGHT_DATA,	UBuffers.DLight);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_PLIGHT_DATA,	UBuffers.PLight);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_SLIGHT_DATA,	UBuffers.SLight);


	StageLightData(cmdList);

	RenderGI(cmdList);
	RenderShadowmaps(cmdList);
	RenderGBuffer(cmdList);
	//RenderTransluscency(cmdList);
	RenderLighting(cmdList);
	RenderPostProcess(cmdList);



	Present(cmdList);


}
void DeferredRenderer::EndRender(FRICommandList& cmdList)
{


}

void DeferredRenderer::RenderGI(FRICommandList& cmdList)
{
	vxgiRenderer.Render(cmdList, this);
}

void DeferredRenderer::RenderGeometry(FRICommandList& cmdList)
{
	Geometry = scene->RegisterSystem<FRenderSystemGeom>(ECSExecutionFlag::USER_TICK);
	Geometry->ForEach([&](Entity ent, Mesh& mesh, Material& material, FTransform& transformComponent)
		{

			if (material.Properties.HasTransluscent)
				return;

			/* Stage DataBuffer */
			cmdList.StageResourcesLambda(UBuffers.Transform, [&](FRIMemoryMap& stageMem)
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
			cmdList.StageResourcesLambda(UBuffers.Material, [&](FRIMemoryMap& stageMem)
				{
					material.StageMemory(stageMem);
				});

			/* Draw */

			cmdList.SetGeometrySource(mesh.VertexBuffer);
			cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, mesh.IndexBuffer->IndexCount, EFRIIndexType::UInt32, mesh.IndexBuffer);
		}

	);

}

void DeferredRenderer::RenderGeometrySkinned(FRICommandList& cmdList)
{
	SkinnedGeometry = scene->RegisterSystem<FRenderSystemSkinnedGeom>(ECSExecutionFlag::USER_TICK);
	SkinnedGeometry->ForEach([&](Entity ent, SkinnedMesh& skinnedMesh, Material& material, FTransform& transformComponent)
		{
			if (material.Properties.HasTransluscent)
				return;

			/* Stage DataBuffer */
			cmdList.StageResourcesLambda(UBuffers.Transform, [&](FRIMemoryMap& stageMem)
				{
					FMatrix4 tmpWorld = transformComponent.GetMatrix();

					stageMem.Load(tmpWorld);
					stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

				});
			cmdList.StageResourcesLambda(UBuffers.JointData, [&](FRIMemoryMap& stageMem)
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
			cmdList.StageResourcesLambda(UBuffers.Material, [&](FRIMemoryMap& stageMem)
				{
					material.StageMemory(stageMem);
				});

			/* Draw */

			cmdList.SetGeometrySource(skinnedMesh.VertexBuffer);
			cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, skinnedMesh.IndexBuffer->IndexCount, EFRIIndexType::UInt32, skinnedMesh.IndexBuffer);
		}

	);
}


void DeferredRenderer::RenderEnvironmentStatic(FRICommandList& cmdList)
{

	cmdList.StageResourcesLambda(UBuffers.Transform, [&](FRIMemoryMap& stageMem)
		{
			stageMem.Load(FMatrix4::Identity());
			stageMem.Load(FMatrix4::Identity());
		});

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

		cmdList.StageResourcesLambda(UBuffers.Material, [&](FRIMemoryMap& stageMem)
			{
				geom->Material.StageMemory(stageMem);
			});

		cmdList.SetGeometrySource(geom->VertexBuffer);
		cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, geom->IndexBuffer->IndexCount, EFRIIndexType::UInt32, geom->IndexBuffer);
	}
}


void DeferredRenderer::RenderGBuffer(FRICommandList& cmdList)
{

	cmdList.SetRasterizerState(DefaultRasterizer);
	cmdList.SetBlendState(DefaultBlend);
	cmdList.GetDynamic()->SetDepthStencilState(DefaultDepth);


	CameraComponent& CameraRef = scene->Camera.Component<CameraComponent>();

	cmdList.StageResourcesLambda(UBuffers.CameraMatrix, [&](FRIMemoryMap& stageMem)
		{
			stageMem.Load(CameraRef);
		});


	cmdList.SetViewport(Viewport);
	cmdList.GetDynamic()->ClearBuffer(FrameBuffers.GBuffer, Color::Transparent);
	{
		/* Static Geometry */
		cmdList.SetShaderPipeline(Shaders.GShader);
		RenderEnvironmentStatic(cmdList);
		RenderGeometry(cmdList);

		cmdList.SetShaderPipeline(Shaders.GSkinnedShader);
		RenderGeometrySkinned(cmdList);
	}
	cmdList.UnbindFrameBuffer();
}
void DeferredRenderer::RenderShadowmaps(FRICommandList& cmdList)
{
	DirectionalLight& SunRef = scene->Sun.Component<DirectionalLight>();


	cmdList.SetViewport(ShadowmapViewport);
	cmdList.ClearBuffer(FrameBuffers.SMBuffer, Color::White);
	{
		for (int i = 0; i < SM_CASCADES; i++)
		{
			cmdList.SetFramebufferTextureLayer(FrameBuffers.SMBuffer, i);


			// Stage cascade camera buffer
			cmdList.StageResourcesLambda(UBuffers.CameraMatrix, [&](FRIMemoryMap& stageMem)
				{
					stageMem.Load(&SunRef.FrustumInfo[i].View, sizeof(CameraComponent));
				});

			cmdList.StageResourcesLambda(UBuffers.Transform, [&](FRIMemoryMap& stageMem)
				{
					stageMem.Load(FMatrix4::Identity());
					stageMem.Load(FMatrix4::Identity());

				});


			/*  Static Shadowed Scene  */
			cmdList.SetShaderPipeline(Shaders.SMShader);

			RenderEnvironmentStatic(cmdList);
			RenderGeometry(cmdList);

			/*  Skinned Shadowed Scene  */
			cmdList.SetShaderPipeline(Shaders.SMSkinnedShader);
			RenderGeometrySkinned(cmdList);

			/*
			for (auto& pSystemPair : scene->ParticleSystems)
			{
				pSystemPair.Value->RenderSystem(cmdList, pSystemPair.Key);
			}*/
		}
	}
	cmdList.UnbindFrameBuffer();
}
void DeferredRenderer::RenderTransluscency(FRICommandList& cmdList)
{

}

void DeferredRenderer::RenderLighting(FRICommandList& cmdList)
{
	auto& CameraRef = scene->Camera.Component<CameraComponent>();
	auto& CameraPosRef = scene->Camera.Component<FTransform>();
	auto& SunRef = scene->Sun.Component<DirectionalLight>();


	cmdList.SetShaderSampler(FUniformSampler(LI_SLOT_DEPTH,		FrameBuffers.GBuffer->GetDepthBuffer()));
	cmdList.SetShaderSampler(FUniformSampler(LI_SLOT_NORMAL,	FrameBuffers.Normal));
	cmdList.SetShaderSampler(FUniformSampler(LI_SLOT_ALBEDO,	FrameBuffers.Albedo));
	cmdList.SetShaderSampler(FUniformSampler(LI_SLOT_MR,		FrameBuffers.MetallicRoughness));
	cmdList.SetShaderSampler(FUniformSampler(LI_SLOT_EMISSIVE,	FrameBuffers.Emissive));
	//cmdList.SetShaderSampler(FUniformSampler(LI_SLOT_AO,		FrameBuffers.AOTexture));
	cmdList.SetShaderSampler(FUniformSampler(LI_SLOT_SHADOWMAP, FrameBuffers.ShadowmapArray));

	cmdList.SetShaderSampler(FUniformSampler(LI_SLOT_BRDF,			FrameBuffers.BRDFLut));
	cmdList.SetShaderSampler(FUniformSampler(LI_SLOT_TRANSLUSCENCY, FrameBuffers.Albedo));


	cmdList.SetShaderSampler(FUniformSampler(LI_SLOT_VX_RADIANCE, vxgiRenderer.VoxelFinal));
	for (int i = 0; i < 6; i++)
	{
		cmdList.SetShaderSampler(FUniformSampler(LI_SLOT_VX_ANISO + i, vxgiRenderer.VoxelAnisotropic[i]));
	}

	cmdList.SetShaderPipeline(Shaders.Lighting);
	cmdList.ClearBuffer(FrameBuffers.Lighting, Color::CornflowerBlue);
	{
		//atmosphereRenderer.RenderSkySphere(cmdList, CameraRef, SunRef);


		cmdList.SetBlendState(DefaultBlend);
		cmdList.SetRasterizerState(DefaultRasterizer);
		cmdList.GetDynamic()->SetDepthStencilState(DisableDepth);

		FRenderUtil::DrawScreenQuad(cmdList);
	}
	cmdList.UnbindFrameBuffer();


	UnbindSamplers(cmdList, 0, 16);

}

void DeferredRenderer::RenderPostProcess(FRICommandList& cmdList)
{


	cmdList.SetRasterizerState(DefaultRasterizer);
	cmdList.SetBlendState(DefaultBlend);
	cmdList.GetDynamic()->SetDepthStencilState(DisableDepth);

	/*smaa.SubmitPass(cmdList, FrameBuffers.LightingTex);
	auto PPInput = smaa.GetOutput();*/
	

	cmdList.ClearBuffer(NULL, Color::Yellow);
	{
		cmdList.SetShaderSampler(FUniformSampler(0, FrameBuffers.LightingTex));

		cmdList.SetShaderPipeline(Shaders.PostProcess);
		FRenderUtil::DrawScreenQuad(cmdList);

		cmdList.SetShaderSampler(FUniformSampler(0));
	}
	cmdList.UnbindFrameBuffer();
}

void DeferredRenderer::Present(FRICommandList& cmdList)
{

}




void DeferredRenderer::StageLightData(FRICommandList& cmdList)
{

	auto DLightSystem = scene->RegisterSystem<FRenderDLight>(ECSExecutionFlag::USER_TICK);
	auto PLightSystem = scene->RegisterSystem<FRenderPLight>(ECSExecutionFlag::USER_TICK);
	auto SLightSystem = scene->RegisterSystem<FRenderSLight>(ECSExecutionFlag::USER_TICK);


	uint32 DLightNum = DLightSystem->Count();
	uint32 PLightNum = PLightSystem->Count();
	uint32 SLightNum = SLightSystem->Count();


	auto& CameraRef = scene->Camera.Component<CameraComponent>();
	auto& SunRef = scene->Sun.Component<DirectionalLight>();

	// Lighting Pass Constants

	FMatrix4 inverseView = FMatrix4::Inverse(CameraRef.View);
	FMatrix4 inverseProjection = FMatrix4::Inverse(CameraRef.Projection);

	cmdList.SetShaderPipeline(Shaders.Lighting);
	cmdList.StageResourcesLambda(UBuffers.LightingConstants, [&](FRIMemoryMap& stageMemory)
		{
			stageMemory << CameraRef.View;
			stageMemory << CameraRef.Projection;
			stageMemory << inverseView;
			stageMemory << inverseProjection;
			stageMemory << inverseView[3];
			stageMemory << 0.1f;
			stageMemory << 300.0f;


			stageMemory << (float)DLightNum;
			stageMemory << (float)PLightNum;
			stageMemory << (float)SLightNum;


		});

	// Cascade Data

	cmdList.StageResourcesLambda(UBuffers.CascadeData, [&](FRIMemoryMap& stageMem)
		{
			for (int i = 0; i < SM_CASCADES; i++)
			{
				//FMatrix4 toLight = SunRef.FrustumInfo[i].View * SunRef.FrustumInfo[i].Projection;

				stageMem << SunRef.FrustumInfo[i].View;
				stageMem << SunRef.FrustumInfo[i].Projection;
				stageMem << FVector4(SunRef.FrustumInfo[i].Depth);
			}
		});


	// Lights Data

	cmdList.StageResourcesLambda(UBuffers.DLight, [&](FRIMemoryMap& stageMem)
		{
			DLightSystem->ForEach([&](Entity en, DirectionalLight& light)
				{
					light.StageMemory(stageMem);
				});

		});

	cmdList.StageResourcesLambda(UBuffers.PLight, [&](FRIMemoryMap& stageMem)
		{
			PLightSystem->ForEach([&](Entity en, PointLight& light)
				{
					light.StageMemory(stageMem);
				});

		});

	cmdList.StageResourcesLambda(UBuffers.SLight, [&](FRIMemoryMap& stageMem)
		{
			SLightSystem->ForEach([&](Entity en, SpotLight& light)
				{
					light.StageMemory(stageMem);
				});

		});
}


void DeferredRenderer::UnbindSamplers(FRICommandList& cmdList, int32 startSlot, int32 endSlot)
{
	for (int i = startSlot; i <= endSlot; i++)
	{
		cmdList.SetShaderSampler(FUniformSampler(i));
	}
}