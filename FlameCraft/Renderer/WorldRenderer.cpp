#include "WorldRenderer.h"
#include <FlameEngine/Core/Framework/IO/FileStream.h>


#include <iostream>


WorldRenderer::WorldRenderer(FRIContext* friContext, World* world) :
	WorldObserver(world),
	FriContext(friContext),
	threadPool(ctpl::thread_pool(std::thread::hardware_concurrency() / 2))
{
	auto allocator = friContext->GetFRIDynamic();

	FAssetManager Content;
	Content.RenderContext = friContext;
	Content.Connect("./Assets/");

	gameTextures = new GameTextureArray(friContext, Content);

	ShaderLibrary lib = Content.Load<ShaderLibrary>("Shaders/mc.fslib");
	chunkShader			= CreateChunkShaderPipeline(allocator, lib.Modules["ChunkGeometry"]);
	//chunkShaderWater	= CreateChunkShaderPipeline(allocator, lib.Modules["ChunkGeometryWater"]);
	//chunkShaderGrass	= CreateChunkShaderPipeline(allocator, lib.Modules["Grass"]);

	waveSettingsBuffer.GPU = allocator->CreateConstantBuffer(16, EFRIAccess::Write, EFRIUsage::Dynamic);

	BitTexture bitTexture = Content.Load<BitTexture>("Materials/mc/grass_displ.flmt");
	windDisplacement = allocator->CreateTexture2D(256, 256, 1, EFRIAccess::None, EFRITextureFormat::RGBA8UNORM, &bitTexture.DataDescriptor);
	sampler = allocator->CreateSamplerState(EFRITextureFilter::Point, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat);

}

FRIPipelineStateObject* WorldRenderer::CreateChunkShaderPipeline(FRIDynamicAllocator* Allocator, const ShaderLibraryModule& Shaders)
{
	FRIInputLayout StaticLitLayout = {
	.NumElements = 5,
	.DeclarationElements = new FRIInputElementDesc[5]
	{
		FRIInputElementDesc("POSITION", EFRIAttributeType::Float3, 0, EFRIAttribUsage::PerVertex),
		FRIInputElementDesc("NORMAL", EFRIAttributeType::Float3, 1, EFRIAttribUsage::PerVertex),
		FRIInputElementDesc("TANGENT", EFRIAttributeType::Float3, 2, EFRIAttribUsage::PerVertex),
		FRIInputElementDesc("BITANGENT", EFRIAttributeType::Float3, 3, EFRIAttribUsage::PerVertex),
		FRIInputElementDesc("TEXCOORD", EFRIAttributeType::Float3, 4, EFRIAttribUsage::PerVertex)
	}
	};

	// Texture 0

	FRIDescriptorRange Texture0Range;
	Texture0Range.NumDescriptors = 4;
	Texture0Range.BaseShaderRegister = 0;
	Texture0Range.RegisterSpace = 0;
	Texture0Range.OffsetInDescriptorsFromTableStart = 0xffffffff;
	Texture0Range.RangeType = EFRIRootDescriptorRangeType::SRV;

	FRIRootDescriptorTable Textures;
	Textures.NumRanges = 1;
	Textures.Ranges = &Texture0Range;

	// CBV

	FRIDescriptorRange ConstantBuffer1Range;
	ConstantBuffer1Range.NumDescriptors = 2;
	ConstantBuffer1Range.BaseShaderRegister = 0;
	ConstantBuffer1Range.RegisterSpace = 0;
	ConstantBuffer1Range.OffsetInDescriptorsFromTableStart = 0xffffffff;
	ConstantBuffer1Range.RangeType = EFRIRootDescriptorRangeType::CBV;

	FRIRootDescriptorTable ConstantBufferTable;
	ConstantBufferTable.NumRanges = 1;
	ConstantBufferTable.Ranges = &ConstantBuffer1Range;

	return Allocator->CreatePipelineStateObject(
		StaticLitLayout,
		{
			FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Vertex, ConstantBufferTable),
			FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Pixel, ConstantBufferTable),
			FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Pixel, Textures),
		},
		{
			FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 0, 0),
			FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 1, 0),
			FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 2, 0),
			FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 3, 0),
		},
		Shaders,
		{
			EFRITextureFormat::RGBA8UNORM,
			EFRITextureFormat::RGBA8UNORM,
			EFRITextureFormat::RGBA8UNORM,
			EFRITextureFormat::RGBA8UNORM
		},
		4
		);

}

void WorldRenderer::AddToCmdList(FRICommandList& cmdList, GRenderMode renderMode)
{
	if (renderMode == GRenderMode::Material)
	{
		cmdList.SetPipelineState(chunkShader);
		cmdList.SetShaderResourceViewPS(0, gameTextures->view);
		//cmdList.SetShaderSamplerState(0, sampler);
	}
	/*else if (renderMode == GRenderMode::Transluscent)
	{
		cmdList.SetShaderPipeline(chunkShaderWater);
		cmdList.StageResourcesLambda(waveSettingsBuffer, [&](FRIMemoryMap& memory)
			{
				auto time = FTime::GetTimestamp();

				auto lt = time.GetSeconds();
				if (lt > 2000000)
				{
					lt -= 2000000.0;
				}

				memory << (float)lt;
				memory << (float)lt;
				memory << (float)lt;
				memory << (float)lt;
			});

		cmdList.SetShaderConstantBuffer(12, waveSettingsBuffer.GPU);
	}*/

	for (auto& [chunkPos, mesh] : meshes)
	{
		auto toChunk3 = (FVector3)World::ChunkToGlobal(chunkPos) - _World->playerPos;
		auto toChunk = FVector2(toChunk3.x, toChunk3.z);

		auto toView3 = _World->playerLookDir;
		auto toView = FVector2(toView3.x, toView3.z);

		if ((FVector2::Dot(toChunk, toView) > 0 || toChunk.Length() < 24.0f) && toChunk.Length() < 16.0f * 30.0f)
		{
			if (renderMode == GRenderMode::Transluscent)
			{
				//mesh->RenderWater(cmdList);
			}
			else
			{
				mesh->RenderStatic(cmdList);
			}
		}
	}

	/*if (renderMode != GRenderMode::Transluscent)
	{
		cmdList.SetShaderPipeline(chunkShaderGrass);
		cmdList.SetShaderResource(0, gameTextures->friTexture->View());
		cmdList.SetShaderResource(1, windDisplacement->View());


		cmdList.StageResourcesLambda(waveSettingsBuffer, [&](FRIMemoryMap& memory)
			{
				auto time = FTime::GetTimestamp();
				memory << time.GetSeconds();
				memory << time.GetSeconds();
				memory << time.GetSeconds();
				memory << time.GetSeconds();
			});

		cmdList.SetShaderConstantBuffer(12, waveSettingsBuffer.GPU);

		for (auto& [chunkPos, mesh] : meshes)
		{
			auto toChunk3 = (FVector3)World::ChunkToGlobal(chunkPos) - _World->playerPos;
			auto toChunk = FVector2(toChunk3.x, toChunk3.z);

			auto toView3 = _World->playerLookDir;
			auto toView = FVector2(toView3.x, toView3.z);

			if ((FVector2::Dot(toChunk, toView) > 0 || toChunk.Length() < 24.0f) && toChunk.Length() < 16.0f * 30.0f)
			{
				mesh->RenderGrass(cmdList);
			}
		}
	}*/
}

void WorldRenderer::BlockChanged(Block oldBlock, Block newBlock, IVector3 globalPos)
{
	auto chunk = World::GlobalToChunkCoord(globalPos);
	auto block = World::GlobalToChunk(globalPos);

	ChunkScheduledUpdate(chunk, true);

	if (block.x == 0)	ChunkScheduledUpdate(chunk - IVector2(1, 0), true);
	if (block.x == 15)	ChunkScheduledUpdate(chunk + IVector2(1, 0), true);
	if (block.z == 0)	ChunkScheduledUpdate(chunk - IVector2(0, 1), true);
	if (block.z == 15)	ChunkScheduledUpdate(chunk + IVector2(0, 1), true);

}

void WorldRenderer::ChunkScheduledUpdate(IVector2 chunkCoord, bool execInThreadPool)
{
	auto start = clock();

	if (!meshes.Contains(chunkCoord))
		meshes[chunkCoord] = new ChunkMesh(_World, chunkCoord);


	if (!tasks.Contains(chunkCoord))
	{
		auto chunkToMesh = _World->ChunkData[chunkCoord];
		auto chunkMeshPtr = meshes[chunkCoord];

		if (execInThreadPool)
		{
			tasks[chunkCoord] = new ChunkMeshFuture(threadPool.push([=](int thrdId)
				{
					auto desc = chunkMeshPtr->RegenerateGeometry(chunkToMesh);
					desc.loaded = false;
					return desc;
				}));
		}
		else
		{
			auto desc = chunkMeshPtr->RegenerateGeometry(chunkToMesh);
			desc.loaded = false;

			std::promise<ChunkMeshDescription> promise;
			tasks[chunkCoord] = new ChunkMeshFuture(promise.get_future());
			promise.set_value(desc);
			
		}
	}
}

void WorldRenderer::FlushFutures()
{
	FArray<IVector2> iTaskDeleteQueue;

	for (auto& [chunkCoord, future] : tasks)
	{
		if (future->_Is_ready())
		{
			auto desc = future->get();
			desc.mesh->StageGeometry(FriContext, desc, vertexDecl);
			if (desc.loaded)
				meshes[desc.mesh->position] = desc.mesh;

			iTaskDeleteQueue.Add(chunkCoord);
		}
	}

	for (auto& vec : iTaskDeleteQueue)
	{
		delete tasks[vec]; // ~future block main thread ..
		tasks.Delete(vec);
	}
}

void WorldRenderer::WorldTick()
{
	FlushFutures();
}