#include "EnvironmentMap.h"
#include "ShaderLibrary.h"
#include "Mesh.h"

#include "../Client/LocalAssetManager.h"

enum class ColorChannels
{
	Red = 1,
	Green = 2,
	Blue = 4,
	Alpha = 8
};


struct FEnvironmentMapFaceHeader
{
	uint32 DimX;
	uint32 DimY;
	uint64 ByteSize;
};

void RenderIrradiance(FRIContext* renderContext, FRITextureCubeMap* data, FRITextureCubeMap* specularIrradiance, int envSize)
{

	/*FAssetManager Content;
	Content.RenderContext = renderContext;
	Content.Connect("./Assets/");

	FRICommandList cmdList(renderContext->GetFRIDynamic());

	FPerspectiveMatrix persp(PI / 2.0f, 1, 0.1f, 20.0f);
	persp = persp * FScalingMatrix(-1.0f, 1, 1);

	FViewMatrix captureViews[] =
	{
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f,  0.0f,  0.0f), FVector3(0.0f, 1.0f,  0.0f)),
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(-1.0f,  0.0f,  0.0f),FVector3(0.0f, 1.0f,  0.0f)),
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f,  1.0f,  0.0f), FVector3(0.0f,  0.0f,  -1.0f)),
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, -1.0f,  0.0f), FVector3(0.0f,  0.0f,  1.0f)),
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f,  0.0f,  1.0f), FVector3(0.0f, 1.0f,  0.0f)),
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f,  0.0f,  -1.0f), FVector3(0.0f, 1.0f,  0.0f)) 
	};


	ShaderLibrary shaderLib = Content.Load<ShaderLibrary>("Shaders/irradiance.fslib");
	auto irrShader = cmdList.GetDynamic()->CreateShaderPipeline(shaderLib.Modules["Irradiance"]);
	auto irrSpecShader = cmdList.GetDynamic()->CreateShaderPipeline(shaderLib.Modules["IrradianceSpecular"]);
	auto mesh = Content.Load<Mesh>("Models/cube.fl3d");

	//auto frameBuffer = cmdList.GetDynamic()->CreateFrameBuffer(diffuseIrradiance, true, 0);

	FRIFrameBuffer* frameBufferSpec[8];
	for (int i = 0; i < 8; i++)
	{
		frameBufferSpec[i] = cmdList.GetDynamic()->CreateFrameBuffer(specularIrradiance, true, i);

		int a = 5;
	}

	FRIStageBuffer uniBuff;
	FRIStageBuffer uniBuff2;
	uniBuff.GPU = cmdList.GetDynamic()->CreateConstantBuffer(sizeof(FMatrix4) * 2, EFRIAccess::Write, EFRIUsage::Dynamic);
	uniBuff2.GPU = cmdList.GetDynamic()->CreateConstantBuffer(sizeof(FMatrix4) * 2 + sizeof(FVector4), EFRIAccess::Write, EFRIUsage::Dynamic);

	auto DefaultBlend = cmdList.GetDynamic()->CreateBlendState(EFRIBlend::Src, EFRIBlend::OneMinusSrc, EFRIBlend::Src, EFRIBlend::Dst);
	auto DisableDepth = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::False, EFRIBool::False);
	auto SMRasterizer = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::None, EFRIFillMode::Solid);
	auto linearSampler = cmdList.GetDynamic()->CreateSamplerState(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat);

	cmdList.SetRasterizerState(SMRasterizer);
	cmdList.SetBlendState(DefaultBlend);
	cmdList.SetDepthStencilState(DisableDepth);

	/*
	cmdList.SetViewport(FViewportRect(0, 0, envSize, envSize));
	cmdList.SetShaderConstantBuffer(0, uniBuff.GPU);
	cmdList.ClearBuffer(frameBuffer, Color::Black);
	{
		cmdList.SetShaderPipeline(irrShader);

		for (int i = 0; i < 6; i++)
		{
			cmdList.SetFramebufferTextureLayer(frameBuffer, i);
			cmdList.StageResourcesLambda(uniBuff, [=](FRIMemoryMap& stageMemory)
				{
					stageMemory << captureViews[i];
					stageMemory << persp;
				});

			cmdList.SetShaderSampler(FUniformSampler(0, data));

			cmdList.SetGeometrySource(mesh.VertexBuffer);
			cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, mesh.IndexBuffer->IndexCount, EFRIIndexType::UInt32, mesh.IndexBuffer);

		}
	}
	cmdList.UnbindFrameBuffer();

	cmdList.SetShaderSamplerState(0, linearSampler);

	for (int mip = 0; mip < 8; mip++)
	{
		float mipSize = envSize / powf(2, mip);

		cmdList.SetViewport(FViewportRect(0, 0, mipSize, mipSize));

		cmdList.SetShaderConstantBuffer(0, uniBuff2.GPU);
		cmdList.ClearBuffer(frameBufferSpec[mip], Color::Black);
		{
			cmdList.SetShaderPipeline(irrSpecShader);

			for (int i = 0; i < 6; i++)
			{
				cmdList.SetFramebufferActiveLayer(frameBufferSpec[mip], i);
				cmdList.StageResourcesLambda(uniBuff2, [=](FRIMemoryMap& stageMemory)
					{
						stageMemory << captureViews[i];
						stageMemory << persp;
						stageMemory << (float)mip / 7.0f;
						stageMemory << (float)mip / 7.0f;
						stageMemory << (float)mip / 7.0f;
						stageMemory << (float)mip / 7.0f;
					});

				cmdList.SetShaderResource(0, data->View());
				mesh.AddToRenderList(cmdList, EFRIPrimitiveType::Triangles);

			}
		}
		cmdList.UnbindFrameBuffer();
	}

	for (int i = 0; i <= 3; i++)
	{
		cmdList.SetShaderResource(i, NULL);
	}*/
}

FRICreationDescriptor TContentSerializer<EnvironmentMap>::ReadMap(Stream& fileStream, int& width, int& height)
{
	FRICreationDescriptor mapData;
	FEnvironmentMapFaceHeader mapHeader = fileStream.Read<FEnvironmentMapFaceHeader>();

	mapData.ByteSize = mapHeader.ByteSize;
	mapData.DataArray = Memory::Alloc<void>(mapHeader.ByteSize);

	fileStream._InternalRead(mapData.DataArray, mapData.ByteSize);

	width = mapHeader.DimX;
	height = mapHeader.DimY;
	
	return mapData;
}

EnvironmentMap TContentSerializer<EnvironmentMap>::Serialize(Stream& fileStream)
{
	FRIDynamicAllocator* allocator = renderContext->GetFRIDynamic();

	int baseWidth = 0;
	int baseHeight = 0;

	int irrWidth = 0;
	int irrHeight = 0;

	int origWidth = 0;
	int origHeight = 0;

	const int envMipCount = 8;

	FRICreationDescriptor envSkyMapData[6];
	FRICreationDescriptor envSpecData[6 * envMipCount];
	FRICreationDescriptor envIrrData[6];

	for (int face = 0; face < 6; face++)
	{
		envSkyMapData[face] = ReadMap(fileStream, origWidth, origHeight); // Read face
	}

	for (int face = 0; face < 6; face++)
	{
		envSpecData[face * 8] = ReadMap(fileStream, baseWidth, baseHeight); // Read Base evel Mip data

		for (int mip = 1; mip < envMipCount; mip++)
		{
			size_t ByteSize = envSpecData[face * 8].ByteSize / powf(4, mip);
			void* data = Memory::Alloc<void>(ByteSize);
			Memory::Zero(data, ByteSize);

			envSpecData[face * 8 + mip] = FRICreationDescriptor(data, ByteSize);
		}
	}

	for (int i = 0; i < 6; i++)
	{
		envIrrData[i] = ReadMap(fileStream, irrWidth, irrHeight); // Read face
	}

	auto envBaseCubeMap = allocator->CreateTextureCubeMap(
		baseWidth,
		baseHeight,
		8,
		EFRIAccess::None,
		EFRITextureFormat::RGBA16UNORM,
		envSpecData
	);

	auto envIrrCubeMap = allocator->CreateTextureCubeMap(
		irrWidth,
		irrHeight,
		1,
		EFRIAccess::None,
		EFRITextureFormat::RGBA16UNORM,
		envIrrData
	);

	auto envSpecFilteredCubeMap = allocator->CreateTextureCubeMap(
		baseWidth,
		baseHeight,
		8,
		EFRIAccess::None,
		EFRITextureFormat::RGBA16F
	);

	auto envSkyCubeMap = allocator->CreateTextureCubeMap(
		origWidth,
		origHeight,
		1,
		EFRIAccess::None,
		EFRITextureFormat::RGBA32F,
		envSkyMapData
	);

	//allocator->FlushMipMaps(envBaseCubeMap->View());

	RenderIrradiance(renderContext, envBaseCubeMap, envSpecFilteredCubeMap, baseWidth);

	for (auto idx : FRange(48))
		Memory::Free(envSpecData[idx].DataArray);
	for (auto idx : FRange(6))
		Memory::Free(envIrrData[idx].DataArray);

	for (auto idx : FRange(6))
		Memory::Free(envSkyMapData[idx].DataArray);

	return EnvironmentMap(envSkyCubeMap, envSpecFilteredCubeMap, envIrrCubeMap);
}