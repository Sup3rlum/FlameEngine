#include "RenderStages.h"


#include "../../ContentSystem/Client/AssetImportScripts/ShaderLibrary.h"
#include "../VXGI/Buffers.h"

/*
struct FTransformBufferStruct
{
	FMatrix4 World;
	FMatrix4 WorldInverseTranspose;
};

struct FMaterialBufferStruct
{
	FVector4 Emissive;
	uint32 HasNormalMap;
	uint32 HasPOMEnabled;
	uint32 HasAOMap;
	uint32 HasEmissive;
	uint32 HasTransluscent;
};
*/

/*
struct FCameraBufferStruct
{
	FViewMatrix View;
	FProjectionMatrix Projection;
	FMatrix4 InverseView;
	FVector4 CameraPosition;
};

uint32 NumFrames = 0;


void DRStageVXGI::CreateResources(ShaderLibrary& s, FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());

	// Create GBuffer Textures

	auto Viewport = renderContext->GetViewport();
	IVector2 viewportSize = Viewport.Size;

	ShaderLibrary Shaders = FLocalContent::LoadFromLocal<ShaderLibrary>("shaders/vxgi.fslib", renderContext);

	voxelizePipeline = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["VoxelizationGen"]);
	lightCompute = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["LightCompute"]);
	flushCompute = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["VoxelFlush"]);
	anisoCompute = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["AnisotropicCreate"]);
	propagationCompute = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["LightPropagation"]);

	CameraMatrixBuffer = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FCameraBufferStruct)));
	TransformBuffer = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FTransformBufferStruct)));
	MaterialBuffer = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FMaterialBufferStruct)));

	VXGIBuffer = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FVXGIBuffer)));
	PointLightBuffer = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(PointLight) * 4));
	DirLightBuffer = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FVector4)));


	float sceneDimension = 100.0f;
	float halfDim = sceneDimension / 2.0f;

	voxelizeProjection = FOrthographicMatrix(-halfDim, halfDim, halfDim, -halfDim, 0, sceneDimension);



	voxelAlbedo = cmdList.GetDynamic()->CreateTexture3D(256, 256, 256, EFRITextureFormat::R32UI, FRIColorDataFormat(EFRIChannels::R, EFRIPixelStorage::UnsignedInt), FRICreationDescriptor(0, 0), false, false);
	voxelNormal = cmdList.GetDynamic()->CreateTexture3D(256, 256, 256, EFRITextureFormat::R32UI, FRIColorDataFormat(EFRIChannels::R, EFRIPixelStorage::UnsignedInt), FRICreationDescriptor(0, 0), false, false);
	voxelEmission = cmdList.GetDynamic()->CreateTexture3D(256, 256, 256, EFRITextureFormat::R32UI, FRIColorDataFormat(EFRIChannels::R, EFRIPixelStorage::UnsignedInt), FRICreationDescriptor(0, 0), false, false);
	voxelRadiance = cmdList.GetDynamic()->CreateTexture3D(256, 256, 256, EFRITextureFormat::RGBA8UNORM, FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::UnsignedByte), FRICreationDescriptor(0, 0), false, false);
	voxelFirstBounce = cmdList.GetDynamic()->CreateTexture3D(256, 256, 256, EFRITextureFormat::RGBA8UNORM, FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::UnsignedByte), FRICreationDescriptor(0, 0), false, false);

	for (int i = 0; i < 6; i++)
	{
		DirectionalVolumes[i] = cmdList.GetDynamic()->CreateTexture3D(128, 128, 128, EFRITextureFormat::RGBA8UNORM);
	}

	outputTex = cmdList.GetDynamic()->CreateTexture2D(1024, 1024, 0, EFRITextureFormat::RGBA8UNORM);
	framebuffer = cmdList.GetDynamic()->CreateFrameBuffer({ FRIFrameBufferAttachment(outputTex) }, false);

	BlendState = cmdList.GetDynamic()->CreateBlendState(EFRIAlphaBlend::Src, EFRIAlphaBlend::OneMinusSrc);
	DepthStencilState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);
	RasterizerState = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::None, EFRIFillMode::Solid);

	FinalDepth = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::True, EFRIBool::False);


	VoxelizeViewport = FViewportRect(0, 0, 1024, 1024);
	OriginalViewport = renderContext->GetViewport();

}

void DRStageVXGI::RecreateResources(ShaderLibrary& Shaders, FRIContext* context, FRIContext* previousContext)
{

}


void DRStageVXGI::Prepare(FRICommandList& cmdList, RStageInterface& input)
{


}
void DRStageVXGI::SubmitPass(FRICommandList& cmdList, Scene* scene)
{
	if (NeedsUpdate)
	{
		Voxelize(cmdList, scene);
		NeedsUpdate = false;
	}
}


void DRStageVXGI::Voxelize(FRICommandList & cmdList, Scene* scene)
{
	cmdList.SetBlendState(BlendState);
	cmdList.SetRasterizerState(RasterizerState);
	cmdList.GetDynamic()->SetDepthStencilState(DepthStencilState);


	cmdList.SetShaderUniformBuffer(UBO_SLOT_CAMERA, CameraMatrixBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_TRANSFORM, TransformBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_MATERIAL, MaterialBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_POINT_LIGHT, PointLightBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_DIR_LIGHT, DirLightBuffer);
	cmdList.SetShaderUniformBuffer(UBO_SLOT_VXGI, VXGIBuffer);


	AABB volume = scene->GetAABB();
	float sceneDimension = fmaxf(fmaxf(volume.LengthX(), volume.LengthY()), volume.LengthZ());

	FVXGIBuffer buffer;
	buffer.maxTracingDistanceGlobal = 1.0f;
	buffer.bounceStrength = 2.0;
	buffer.aoFalloff = 725.0;
	buffer.aoAlpha = 0.01f;
	buffer.samplingFactor = 0.5f;
	buffer.coneShadowTolerance = 1.0f;
	buffer.coneShadowAperture = 0.03f;

	buffer.voxelVolumeDimension = 256.0f;
	buffer.sceneDimension = sceneDimension;
	buffer.voxelScale = 1.0f / buffer.sceneDimension;
	buffer.voxelSize = buffer.sceneDimension / buffer.voxelVolumeDimension;
	buffer.WorldMin = FVector4(volume.minPoint, 1);
	buffer.WorldMax = FVector4(volume.maxPoint, 1);

	cmdList.StageResourcesLambda(VXGIBuffer, [&](FRIMemoryMap& stageMemory)
		{
			stageMemory.Load(buffer);
		});


	auto Geometry = scene->RegisterSystem<FRenderSystemGeom>(ECSExecutionFlag::USER_TICK);

	FlushVolumes(cmdList);

	CameraComponent& CameraRef = scene->Camera.Component<CameraComponent>();
	CameraComponent voxelCam = CameraRef;
	voxelCam.Projection = voxelizeProjection;

	cmdList.SetViewport(VoxelizeViewport);
	cmdList.StageResourcesLambda(CameraMatrixBuffer, [&](FRIMemoryMap& stageMem)
		{
			FMatrix4 inverseView = FMatrix4::Inverse(voxelCam.View);

			stageMem.Load(voxelCam.View);
			stageMem.Load(voxelCam.Projection);
			stageMem.Load(inverseView);
			stageMem.Load(FVector4(inverseView[3].xyz, 1.0f));
		});

	cmdList.StageResourcesLambda(DirLightBuffer, [&](FRIMemoryMap& stageMem)
		{
			stageMem.Load(FVector4(scene->Sun.Component<DirectionalLight>().Direction, 1));
		});


	// Present
	cmdList.GetDynamic()->ClearBufferAndSetUAVs(framebuffer, Color::Black, new FRITexture3D * [] {voxelAlbedo, voxelNormal, voxelEmission}, 3);
	{

		cmdList.SetShaderPipeline(voxelizePipeline);


		cmdList.StageResourcesLambda(TransformBuffer, [&](FRIMemoryMap& stageMem)
			{
				stageMem.Load(FMatrix4::Identity());
				stageMem.Load(FMatrix4::Identity());
			});


		for (auto& geom : scene->SceneLevel.LevelGeometry.Leafs)
		{

			/* Stage Samplers 
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

			cmdList.SetGeometrySource(geom->VertexBuffer);
			cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, geom->IndexBuffer->IndexCount, EFRIIndexType::UInt32, geom->IndexBuffer);
		}



		Geometry->ForEach([&](Entity ent, Mesh& mesh, Material& material, FTransform& transformComponent)
			{

				if (material.Properties.HasTransluscent)
					return;

				/* Stage DataBuffer 
				cmdList.StageResourcesLambda(TransformBuffer, [&] (FRIMemoryMap& stageMem)
					{
						FMatrix4 tmpWorld = transformComponent.GetMatrix();

						stageMem.Load(tmpWorld);
						stageMem.Load(FMatrix4::Inverse(FMatrix4::Transpose(tmpWorld)));

					});


				/* Stage Samplers 
				for (int map = 0; map < (uint32)EMaterialMap::MAX_MAPS; map++)
				{
					FRITexture2D* textureMap = material.GetMap((EMaterialMap)map).Handle;
					if (textureMap)
					{
						cmdList.SetShaderSampler(FUniformSampler(map, textureMap));
					}
				}

				/* Stage emission properties 
				cmdList.StageResourcesLambda(MaterialBuffer, [&](FRIMemoryMap& stageMem)
					{
						material.StageMemory(stageMem);
					});

				/* Draw 

				cmdList.SetGeometrySource(mesh.VertexBuffer);
				cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, mesh.IndexBuffer->IndexCount, EFRIIndexType::UInt32, mesh.IndexBuffer);
			}

		);
	}

	cmdList.UnbindFrameBuffer();
	cmdList.SetViewport(OriginalViewport);


	InjectRadiance(cmdList, scene);
	GenerateMipMaps(cmdList, voxelRadiance);

	if (getFirstBounce)
	{
		InjectFirstBounce(cmdList);
		GenerateMipMaps(cmdList, voxelFirstBounce);
	}
}
void DRStageVXGI::Finish(FRICommandList & cmdList, RStageInterface & rso)
{
	if (getFirstBounce)
		rso.Buffer[RS_SLOT_RADIANCE] = voxelFirstBounce;
	else
		rso.Buffer[RS_SLOT_RADIANCE] = voxelRadiance;


	for (int i = 0; i < 6; i++)
	{
		rso.Buffer[RS_SLOT_VOXEL_ANISO + i] = DirectionalVolumes[i];
	}
}


void DRStageVXGI::FlushVolumes(FRICommandList& cmdList)
{
	cmdList.SetBlendState(BlendState);
	cmdList.SetRasterizerState(RasterizerState);
	cmdList.GetDynamic()->SetDepthStencilState(DepthStencilState);

	cmdList.GetDynamic()->SetUAV(0, voxelAlbedo);
	cmdList.GetDynamic()->SetUAV(1, voxelNormal);
	cmdList.GetDynamic()->SetUAV(2, voxelEmission);
	cmdList.GetDynamic()->SetUAV(3, voxelRadiance);
	cmdList.GetDynamic()->SetUAV(4, voxelFirstBounce);

	cmdList.SetShaderPipeline(flushCompute);
	{
		cmdList.GetDynamic()->DispatchCompute(32, 32, 32);
	}

	cmdList.GetDynamic()->SetUAV(0, NULL);
	cmdList.GetDynamic()->SetUAV(1, NULL);
	cmdList.GetDynamic()->SetUAV(2, NULL);
	cmdList.GetDynamic()->SetUAV(3, NULL);
	cmdList.GetDynamic()->SetUAV(4, NULL);

}

void DRStageVXGI::GenerateMipMaps(FRICommandList& cmdList, FRITexture3D* voxelmap)
{
	// Mipmap Base

	cmdList.SetShaderPipeline(anisoCompute);
	{

		cmdList.SetShaderSampler(FUniformSampler(0, voxelmap));
		for (int i = 0; i < 6; i++)
		{
			cmdList.GetDynamic()->SetUAV(i, DirectionalVolumes[i]);
		}

		cmdList.GetDynamic()->DispatchCompute(16, 16, 16);

		for (int i = 0; i < 6; i++)
		{
			cmdList.GetDynamic()->SetUAV(i, NULL);
			cmdList.GetDynamic()->FlushMipMaps(DirectionalVolumes[i]);
		}

		cmdList.SetShaderSampler(FUniformSampler(0));
	}

	cmdList.FlushMipMaps(voxelmap);

}

void DRStageVXGI::InjectRadiance(FRICommandList& cmdList, Scene* scene)
{
	CameraComponent& CameraRef = scene->Camera.Component<CameraComponent>();

	cmdList.GetDynamic()->SetUAV(0, voxelAlbedo);
	cmdList.GetDynamic()->SetUAV(1, voxelNormal);
	cmdList.GetDynamic()->SetUAV(2, voxelEmission);
	cmdList.GetDynamic()->SetUAV(3, voxelRadiance);

	cmdList.SetShaderPipeline(lightCompute);
	{

		cmdList.StageResourcesLambda(CameraMatrixBuffer, [&](FRIMemoryMap& stageMem)
			{
				FMatrix4 inverseView = FMatrix4::Inverse(CameraRef.View);

				stageMem.Load(CameraRef.View);
				stageMem.Load(CameraRef.Projection);
				stageMem.Load(inverseView);
				stageMem.Load(FVector4(inverseView[3].xyz, 1.0f));
			});


		cmdList.StageResourcesLambda(PointLightBuffer, [&](FRIMemoryMap& stageMem)
			{
				stageMem.Load(scene->pointLights[0]);
				stageMem.Load(scene->pointLights[1]);
				stageMem.Load(scene->pointLights[2]);
				stageMem.Load(scene->pointLights[3]);
			});

		cmdList.GetDynamic()->DispatchCompute(32, 32, 32);
	}

	cmdList.GetDynamic()->SetUAV(0, NULL);
	cmdList.GetDynamic()->SetUAV(1, NULL);
	cmdList.GetDynamic()->SetUAV(2, NULL);
	cmdList.GetDynamic()->SetUAV(3, NULL);
}

void DRStageVXGI::InjectFirstBounce(FRICommandList& cmdList)
{
	cmdList.GetDynamic()->SetUAV(0, voxelAlbedo);
	cmdList.GetDynamic()->SetUAV(1, voxelNormal);
	cmdList.GetDynamic()->SetUAV(2, voxelRadiance);
	cmdList.GetDynamic()->SetUAV(3, voxelFirstBounce);


	for (int i = 0; i < 6; i++)
	{
		cmdList.SetShaderSampler(FUniformSampler(i, DirectionalVolumes[i]));
	}

	cmdList.SetShaderPipeline(propagationCompute);
	{
		cmdList.GetDynamic()->DispatchCompute(32, 32, 32);
	}

	for (int i = 0; i < 6; i++)
	{
		cmdList.SetShaderSampler(FUniformSampler(i));
	}

	cmdList.GetDynamic()->SetUAV(0, NULL);
	cmdList.GetDynamic()->SetUAV(1, NULL);
	cmdList.GetDynamic()->SetUAV(2, NULL);
	cmdList.GetDynamic()->SetUAV(3, NULL);
}*/