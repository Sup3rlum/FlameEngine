#include "DeferredRenderer.h"


#include "Core/Framework/IO/FileStream.h"
#include "Core/Engine/ContentSystem/Client/LocalAssetManager.h"
#include "../ContentSystem/ImportScripts/Material.h"
#include "../GameSystem/RenderObject.h"

#include "Deferred/BRDF.h"
#include "../Common/FTime.h"

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


void DeferredRenderer::_DRUBuffers::Create(DeferredRenderer* renderer, FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());

	CameraMatrix.GPU	= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(Camera)));
	Transform.GPU		= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FMatrix4) * 2));
	JointData.GPU		= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FMatrix4) * 32));
	Material.GPU		= cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, Material::GetStageMemorySize()));

	CascadeData.GPU = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FViewFrustumInfo) * SM_CASCADES));

	DLight.GPU = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, DirectionalLight::GetStageMemorySize() * 32));
	PLight.GPU = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, PointLight::GetStageMemorySize() * 32));
	SLight.GPU = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, SpotLight::GetStageMemorySize() * 32));

	LightingConstants.GPU = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, 76 * sizeof(float)));
}


void DeferredRenderer::_DRFBO::Create(DeferredRenderer* renderer, FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());
	IVector2 viewportSize = renderContext->GetViewport().Size;

	Albedo				= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);
	Normal				= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);
	MetallicRoughness	= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);
	Emissive			= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);
	
	AOTexture			= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::R8UNORM);
	PostProcessTex		= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA8UNORM);
	LightingTex			= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA16F);
	LightAndTransluscentTex	= cmdList.GetDynamic()->CreateTexture2D(viewportSize.x, viewportSize.y, 0, EFRITextureFormat::RGBA16F);

	auto ShadowmapViewport = IVector2(renderer->Property("ShadowmapResolution").GetInt());
	ShadowmapArray = cmdList.GetDynamic()->CreateTexture2DArray(ShadowmapViewport.x, ShadowmapViewport.y, SM_CASCADES, EFRITextureFormat::RG16F, FRIColorDataFormat(EFRIChannels::RG, EFRIPixelStorage::Float));


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
	AOBuffer	= cmdList.GetDynamic()->CreateFrameBuffer({ FRIFrameBufferAttachment(AOTexture) }, false);
	TransluscentBuffer = cmdList.GetDynamic()->CreateFrameBuffer({ FRIFrameBufferAttachment(LightAndTransluscentTex) }, true);

	BRDFLut = cmdList.GetDynamic()->CreateTexture2D(
		BRDF_LUT_WIDTH,
		BRDF_LUT_HEIGHT,
		0,
		EFRITextureFormat::RG8UNORM,
		FRIColorDataFormat(EFRIChannels::RG, EFRIPixelStorage::UnsignedByte),
		FRICreationDescriptor(brdfTex, BRDF_LUT_SIZE));
}


void DeferredRenderer::_DRShaders::Create(DeferredRenderer* renderer, FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());


	FAssetManager Content;
	Content.RenderContext = renderContext;
	Content.Connect("./Assets/");

	ShaderLibrary deferredLib =  Content.Load<ShaderLibrary>("Shaders/deferred_dx.fslib");

	GShader			= cmdList.GetDynamic()->CreateShaderPipeline(deferredLib.Modules["GBufferTessellated"]);
	GSkinnedShader	= cmdList.GetDynamic()->CreateShaderPipeline(deferredLib.Modules["GBufferSkinned"]);
	Lighting		= cmdList.GetDynamic()->CreateShaderPipeline(deferredLib.Modules["Lighting"]);
	PostProcess		= cmdList.GetDynamic()->CreateShaderPipeline(deferredLib.Modules["PostProcess"]);
	SMShader		= cmdList.GetDynamic()->CreateShaderPipeline(deferredLib.Modules["Shadow"]);
	SMSkinnedShader = cmdList.GetDynamic()->CreateShaderPipeline(deferredLib.Modules["ShadowSkinned"]);
}

void DeferredRenderer::CreateResources(FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());
	Viewport = renderContext->GetViewport();
	ShadowmapViewport = FViewportRect(0, 0, ShadowmapResolution, ShadowmapResolution);

	DefaultBlend = cmdList.GetDynamic()->CreateBlendState(EFRIAlphaBlend::Src, EFRIAlphaBlend::OneMinusSrc);
	DisableDepth = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);
	DefaultDepth = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::True, EFRIBool::False);
	DefaultRasterizer = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);
	SMRasterizer = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::None, EFRIFillMode::Solid);

	CreateRenderUtil(renderContext);

	Shaders.Create(this, renderContext);
	UBuffers.Create(this, renderContext);
	FrameBuffers.Create(this, renderContext);

	atmosphereRenderer.CreateResources(renderContext);
	smaa.CreateResources(renderContext);
	blurRenderer.CreateResources(renderContext);
	particleRenderer.CreateResources(renderContext);


	FHBAOParameters params;

	params.Radius = 2.0f;
	params.Bias = 0.1f;
	params.NearAO = 2.0f;
	params.FarAO = 1.5f;
	
	params.BackgroundAOEnable = false;
	params.BackgroundAO_ViewDepth = 10.0f;
	
	params.ForegroundAOEnable = false;
	params.ForegroundAO_ViewDepth = 1.0f;
	
	params.PowerExponent = 2.0f;
	params.BlurEnable = true;
	params.BlurSharpness = 16.0f;

	hbaoRenderer = HBAOPlus::Allocate(renderContext, params);


	this->RenderModules.Add(&atmosphereRenderer);
	//this->RenderModules.Add(&debugRenderer);
	this->RenderModules.Add(&smaa);
	this->RenderModules.Add(&blurRenderer);

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
	if (renderContext->InstanceDescription.RenderFramework == EFRIRendererFramework::DX11)
	{
		scrQuadSignatureShader = cmdList.GetDynamic()->CreateVertexShader(IOFileStream("Assets/Shaders/signature/dx/bin/CombineQuad.signature.cso").ReadBytes());
	}


	FArray<FRIInputDesc> LayoutDesc;
	LayoutDesc.Add(FRIInputDesc(FRenderUtil::VertexDeclComp, 0));

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
	cmdList.SetShaderUniformBuffer(UBO_SLOT_CAMERA,		UBuffers.CameraMatrix.GPU, EFRI_Vertex | EFRI_Pixel);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_TRANSFORM,	UBuffers.Transform.GPU, EFRI_Vertex);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_JOINTS,		UBuffers.JointData.GPU, EFRI_Vertex);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_MATERIAL,	UBuffers.Material.GPU, EFRI_Vertex | EFRI_Pixel);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_CASCADE,	UBuffers.CascadeData.GPU, EFRI_Vertex | EFRI_Pixel);

	cmdList.SetShaderUniformBuffer(UBO_SLOT_LIGHTING, UBuffers.LightingConstants.GPU, EFRI_Vertex | EFRI_Pixel);

	cmdList.SetShaderUniformBuffer(UBO_SLOT_DLIGHT_DATA, UBuffers.DLight.GPU, EFRI_Pixel);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_PLIGHT_DATA, UBuffers.PLight.GPU, EFRI_Pixel);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_SLIGHT_DATA, UBuffers.SLight.GPU, EFRI_Pixel);

}

#define PROFILE_(name, a, code) auto now_##a = FTime::GetTimestamp(); \
							code \
							a += FTime::GetTimestamp() - now_##a; \

void DeferredRenderer::Render(FRICommandList& cmdList)
{

	if (!scene->Elements.Contains("GameCamera") || !scene->Elements["GameCamera"].HasComponent<Camera>())
		return;
	if (!scene->Elements.Contains("Sun") || !scene->Elements["Sun"].HasComponent<DirectionalLight>())
		return;

	if (!scene->Elements.Contains("Environment") || !scene->Elements["Environment"].HasComponent<EnvironmentMap>())
		return;

	Camera& camera = scene->Elements["GameCamera"].Component<Camera>();
	FTransform& cameraPos = scene->Elements["GameCamera"].Component<FTransform>();
	DirectionalLight& sun = scene->Elements["Sun"].Component<DirectionalLight>();
	EnvironmentMap& env = scene->Elements["Environment"].Component<EnvironmentMap>();


	PROFILE_("Stage", stageTime,
		StageLightData(cmdList, camera, sun);
	)

	PROFILE_("SM", smTime,
		RenderShadowmaps(cmdList, sun);
	)

	PROFILE_("GBuffer", gTime,
		RenderGBuffer(cmdList, camera);
	)

	PROFILE_("GI", giTime,
		RenderGI(cmdList, camera);
	)

	PROFILE_("Lighting", lightTime,
		RenderLighting(cmdList, camera, sun, env);
	)

		RenderTransluscency(cmdList);

	PROFILE_("PostProcess", ppTime,
		RenderPostProcess(cmdList);
	)
}

DeferredRenderer::~DeferredRenderer()
{

}

void DeferredRenderer::EndRender(FRICommandList& cmdList)
{


}

void DeferredRenderer::RenderGI(FRICommandList& cmdList, const Camera& Camera)
{
	hbaoRenderer->RenderAO(FrameBuffers.GBuffer->GetDepthBuffer(), FrameBuffers.Normal, FrameBuffers.AOBuffer, Camera.Projection, Camera.View);
}
void DeferredRenderer::RenderGeometry(FRICommandList& cmdList, GRenderMode renderMode)
{
	scene->System<Model, FTransform>()->CopyForEach([&](Model& model, FTransform& transformComponent)
		{

			if (model.Material.Properties.HasTransluscent)
				return;

			/* Stage DataBuffer */
			cmdList.StageResourcesLambda(UBuffers.Transform, [&](FRIMemoryMap& stageMem)
				{
					FMatrix4 tmpWorld = transformComponent.GetMatrix();

					stageMem.Load(tmpWorld);
					stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

				});


			if (renderMode == GRenderMode::Material)
			{
				/* Stage Samplers */
				for (int map = 0; map < (uint32)EMaterialMap::MAX_MAPS; map++)
				{
					FRITexture2D* textureMap = model.Material.GetMap((EMaterialMap)map).Handle;
					if (textureMap)
					{
						cmdList.SetShaderSampler(FRISampler(map, textureMap));
					}
				}

				/* Stage emission properties */
				cmdList.StageResourcesLambda(UBuffers.Material, [&](FRIMemoryMap& stageMem)
					{
						model.Material.StageMemory(stageMem);
					});
			}
			/* Draw */
			model.AddToRenderList(cmdList);
		}
	);

	scene->System<RenderObject, FTransform>()->ForEach([&](Entity ent, RenderObject& object, FTransform& transformComponent)
		{
			/* Stage DataBuffer */
			cmdList.StageResourcesLambda(UBuffers.Transform, [&](FRIMemoryMap& stageMem)
				{
					FMatrix4 tmpWorld = transformComponent.GetMatrix();

					stageMem.Load(tmpWorld);
					stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

				});

			/* Draw */
			object.list->AddToCmdList(cmdList, renderMode);
		}

	);
}

void DeferredRenderer::RenderGeometrySkinned(FRICommandList& cmdList)
{
	scene->System<SkinnedMesh, Material, FTransform>()->CopyForEach([&](SkinnedMesh& skinnedMesh, Material& material, FTransform& transformComponent)
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
					cmdList.SetShaderSampler(FRISampler(map, textureMap));
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


void DeferredRenderer::RenderEnvironmentStatic(FRICommandList& cmdList, GRenderMode renderMode)
{

	cmdList.StageResourcesLambda(UBuffers.Transform, [&](FRIMemoryMap& stageMem)
		{
			stageMem.Load(FMatrix4::Identity());
			stageMem.Load(FMatrix4::Identity());
		});

	for (auto& geom : scene->SceneLevel.LevelGeometry.Leafs)
	{
		/* Stage Samplers */

		if (renderMode == GRenderMode::Material)
		{
			for (int map = 0; map < (uint32)EMaterialMap::MAX_MAPS; map++)
			{
				FRITexture2D* textureMap = geom->Material.GetMap((EMaterialMap)map).Handle;
				if (textureMap)
				{
					cmdList.SetShaderSampler(FRISampler(map, textureMap));
				}
			}

			cmdList.StageResourcesLambda(UBuffers.Material, [&](FRIMemoryMap& stageMem)
				{
					geom->Material.StageMemory(stageMem);
				});
		}

		cmdList.SetGeometrySource(geom->VertexBuffer);
		cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, geom->IndexBuffer->IndexCount, EFRIIndexType::UInt32, geom->IndexBuffer);
	}
}


void DeferredRenderer::RenderGBuffer(FRICommandList& cmdList, const Camera& camera)
{
	cmdList.SetRasterizerState(DefaultRasterizer);
	cmdList.SetBlendState(DefaultBlend);
	cmdList.SetDepthStencilState(DefaultDepth);

	cmdList.StageResourcesLambda(UBuffers.CameraMatrix, [&](FRIMemoryMap& stageMem)
		{
			stageMem.Load(camera);
		});


	cmdList.SetViewport(Viewport);
	cmdList.ClearBuffer(FrameBuffers.GBuffer, Color::Transparent);
	{
		/* Static Geometry */
		cmdList.SetShaderPipeline(Shaders.GShader);
		RenderEnvironmentStatic(cmdList, GRenderMode::Material);
		RenderGeometry(cmdList, GRenderMode::Material);

		cmdList.SetShaderPipeline(Shaders.GSkinnedShader);
		RenderGeometrySkinned(cmdList);
	}
	cmdList.UnbindFrameBuffer();
}
void DeferredRenderer::RenderShadowmaps(FRICommandList& cmdList, const DirectionalLight& Sun)
{

	cmdList.SetRasterizerState(SMRasterizer);
	cmdList.SetBlendState(DefaultBlend);
	cmdList.SetDepthStencilState(DefaultDepth);

	cmdList.SetViewport(ShadowmapViewport);
	cmdList.ClearBuffer(FrameBuffers.SMBuffer, Color::White);
	{
		cmdList.StageResourcesLambda(UBuffers.Transform, [&](FRIMemoryMap& memory)
			{
				memory.Load(FMatrix4::Identity());
				memory.Load(FMatrix4::Identity());

			});

		for (int i = 0; i < SM_CASCADES; i++)
		{

			cmdList.SetFramebufferTextureLayer(FrameBuffers.SMBuffer, i);

			// Stage cascade camera buffer
			cmdList.StageResourcesLambda(UBuffers.CameraMatrix, [&](FRIMemoryMap& memory)
				{
					memory.Load(&Sun.FrustumInfo[i].View, sizeof(Camera));
				});


			/*  Static Shadowed Scene  */
			cmdList.SetShaderPipeline(Shaders.SMShader);

			RenderEnvironmentStatic(cmdList, GRenderMode::Shadow);
			RenderGeometry(cmdList, GRenderMode::Shadow);

			/*  Skinned Shadowed Scene  */
			//cmdList.SetShaderPipeline(Shaders.SMSkinnedShader);
			//RenderGeometrySkinned(cmdList);
		}
	}
	cmdList.UnbindFrameBuffer();


	//cmdList.GetDynamic()->FlushMipMaps(FrameBuffers.ShadowmapArray);
}

void DeferredRenderer::RenderLighting(
	FRICommandList& cmdList, 
	const Camera& Camera, 
	const DirectionalLight& Sun, 
	const EnvironmentMap& env
)
{
	auto emissiveTexBlurred = blurRenderer.Render(cmdList, FrameBuffers.Emissive);

	cmdList.SetShaderSampler(FRISampler(LI_SLOT_DEPTH,		FrameBuffers.GBuffer->GetDepthBuffer()));
	cmdList.SetShaderSampler(FRISampler(LI_SLOT_NORMAL,	FrameBuffers.Normal));
	cmdList.SetShaderSampler(FRISampler(LI_SLOT_ALBEDO,	FrameBuffers.Albedo));
	cmdList.SetShaderSampler(FRISampler(LI_SLOT_MR,		FrameBuffers.MetallicRoughness));
	cmdList.SetShaderSampler(FRISampler(LI_SLOT_EMISSIVE,	emissiveTexBlurred));
	cmdList.SetShaderSampler(FRISampler(LI_SLOT_AO,		FrameBuffers.AOTexture));
	cmdList.SetShaderSampler(FRISampler(LI_SLOT_SHADOWMAP, FrameBuffers.ShadowmapArray));

	cmdList.SetShaderSampler(FRISampler(LI_SLOT_BRDF,			FrameBuffers.BRDFLut));
	cmdList.SetShaderSampler(FRISampler(8, env.Specular));
	cmdList.SetShaderSampler(FRISampler(9, env.Irradiance));


	cmdList.ClearBuffer(FrameBuffers.Lighting, Color::CornflowerBlue);
	{
		atmosphereRenderer.RenderSkySphere(cmdList, Camera, Sun);

		cmdList.SetShaderPipeline(Shaders.Lighting);

		cmdList.SetBlendState(DefaultBlend);
		cmdList.SetRasterizerState(DefaultRasterizer);
		cmdList.SetDepthStencilState(DisableDepth);

		FRenderUtil::DrawScreenQuad(cmdList);

	}
	cmdList.UnbindFrameBuffer();

	UnbindSamplers(cmdList, 0, 16);
}
void DeferredRenderer::RenderTransluscency(FRICommandList& cmdList)
{
	// Copy Lighting and depth data from Lighting stage onto FrameBuffer
	cmdList.GetDynamic()->CopyTexture(FrameBuffers.LightingTex, FrameBuffers.LightAndTransluscentTex);
	cmdList.GetDynamic()->CopyTexture(FrameBuffers.GBuffer->GetDepthBuffer(), FrameBuffers.TransluscentBuffer->GetDepthBuffer());

	// Set Albedo Normal And Depth to be available for Forward rendering reads (i.e transluscency, refraction, SSR and other screen space effects)
	cmdList.SetShaderSampler(FRISampler(LI_SLOT_DEPTH, FrameBuffers.GBuffer->GetDepthBuffer()));
	cmdList.SetShaderSampler(FRISampler(LI_SLOT_NORMAL, FrameBuffers.Normal));
	cmdList.SetShaderSampler(FRISampler(LI_SLOT_ALBEDO, FrameBuffers.LightingTex));

	cmdList.BindFrameBuffer(FrameBuffers.TransluscentBuffer);
	{
		// Transluscent Instructions
		cmdList.SetBlendState(DefaultBlend);
		cmdList.SetRasterizerState(DefaultRasterizer);
		cmdList.SetDepthStencilState(DefaultDepth);

		scene->System<RenderObject, FTransform>()->ForEach([&](Entity ent, RenderObject& object, FTransform& transformComponent)
			{
				/* Stage DataBuffer*/
				cmdList.StageResourcesLambda(UBuffers.Transform, [&](FRIMemoryMap& stageMem)
					{
						FMatrix4 tmpWorld = transformComponent.GetMatrix();

						stageMem.Load(tmpWorld);
						stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

					});

				/* Draw*/
				object.list->AddToCmdList(cmdList, GRenderMode::Transluscent);
			}

		);

		scene->System<ParticleManager>()->ForEach([&](Entity ent, ParticleManager& particleManager)
			{
				particleRenderer.RenderSystem(cmdList, particleManager.ParticleSystemPtr);
			});

	}
	cmdList.UnbindFrameBuffer();
	UnbindSamplers(cmdList, 0, 16);
}

void DeferredRenderer::RenderPostProcess(FRICommandList& cmdList)
{
	smaa.SubmitPass(cmdList, FrameBuffers.LightAndTransluscentTex);
	auto PPInput = smaa.GetOutput();


	cmdList.SetRasterizerState(DefaultRasterizer);
	cmdList.SetBlendState(DefaultBlend);
	cmdList.SetDepthStencilState(DisableDepth);


	cmdList.ClearBuffer(NULL, Color::Yellow);
	{
		//cmdList.SetShaderSampler(FRISampler(0, FrameBuffers.LightingTex));
		cmdList.SetShaderSampler(FRISampler(0, PPInput));

		cmdList.SetShaderPipeline(Shaders.PostProcess);
		FRenderUtil::DrawScreenQuad(cmdList);

		cmdList.SetShaderSampler(FRISampler(0));
	}
	cmdList.UnbindFrameBuffer();
}

void DeferredRenderer::Present(FRICommandList& cmdList)
{

}




void DeferredRenderer::StageLightData(FRICommandList& cmdList, const Camera& camera, const DirectionalLight Sun)
{

	auto DLightSystem = scene->RegisterSystem<FRenderDLight>(ECSExecutionFlag::USER_TICK);
	auto PLightSystem = scene->RegisterSystem<FRenderPLight>(ECSExecutionFlag::USER_TICK);
	auto SLightSystem = scene->RegisterSystem<FRenderSLight>(ECSExecutionFlag::USER_TICK);

	uint32 DLightNum = DLightSystem->CopyCount();
	uint32 PLightNum = PLightSystem->CopyCount();
	uint32 SLightNum = SLightSystem->CopyCount();

	// Lighting Pass Constants

	FMatrix4 inverseView = FMatrix4::Inverse(camera.View);
	FMatrix4 inverseProjection = FMatrix4::Inverse(camera.Projection);

	cmdList.SetShaderPipeline(Shaders.Lighting);
	cmdList.StageResourcesLambda(UBuffers.LightingConstants, [=](FRIMemoryMap& memory)
		{
			memory
				<< camera.View		
				<< camera.Projection
				<< inverseView
				<< inverseProjection
				<< inverseView[3]
				<< 0.1f
				<< 500.0f		
				<< (float)DLightNum
				<< (float)PLightNum
				<< (float)SLightNum;


		});

	// Cascade Data

	cmdList.StageResourcesLambda(UBuffers.CascadeData, [=](FRIMemoryMap& stageMem)
		{
			for (int i = 0; i < SM_CASCADES; i++)
			{
				//FMatrix4 toLight = SunRef.FrustumInfo[i].View * SunRef.FrustumInfo[i].Projection;

				stageMem << Sun.FrustumInfo[i].View;
				stageMem << Sun.FrustumInfo[i].Projection;
				stageMem << FVector4(Sun.FrustumInfo[i].Depth);
			}
		});


	// Lights Data

	cmdList.StageResourcesLambda(UBuffers.DLight, [=](FRIMemoryMap& stageMem)
		{
			scene->System<DirectionalLight>()->CopyForEach([&](DirectionalLight& light)
				{
					light.StageMemory(stageMem);
				});

		});

	cmdList.StageResourcesLambda(UBuffers.PLight, [=](FRIMemoryMap& stageMem)
		{
			scene->System<PointLight>()->CopyForEach([&](PointLight& light)
				{
					light.StageMemory(stageMem);
				});

		});

	cmdList.StageResourcesLambda(UBuffers.SLight, [&](FRIMemoryMap& stageMem)
		{
			scene->System<SpotLight>()->CopyForEach([&](SpotLight& light)
				{
					light.StageMemory(stageMem);
				});

		});
}


void DeferredRenderer::UnbindSamplers(FRICommandList& cmdList, int32 startSlot, int32 endSlot)
{
	for (int i = startSlot; i <= endSlot; i++)
	{
		cmdList.SetShaderSampler(FRISampler(i));
	}
}
