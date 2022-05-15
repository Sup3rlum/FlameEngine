#include "WorldRenderer.h"
#include <FlameEngine/Core/Framework/IO/FileStream.h>


#include <iostream>


WorldRenderer::WorldRenderer(FRIContext* friContext, World* world) :
	WorldObserver(world),
	FriContext(friContext),
	threadPool(ctpl::thread_pool(std::thread::hardware_concurrency() / 2))
{
	FRICommandList cmdList(friContext->GetFRIDynamic());

	FRIVertexShader* signatureShader = NULL;
	signatureShader = cmdList.GetDynamic()->CreateVertexShader(IOFileStream("Assets/Shaders/signature/dx/bin/ChunkGeometry.signature.cso").ReadBytes());

	FArray<FRIInputAttribute> DeclCompArray =
	{
		FRIInputAttribute("POSITION",		3, EFRIAttributeType::Float, EFRIBool::False, 60, 0),
		FRIInputAttribute("NORMAL",		3, EFRIAttributeType::Float, EFRIBool::False, 60, 12),
		FRIInputAttribute("TANGENT",		3, EFRIAttributeType::Float, EFRIBool::False, 60, 24),
		FRIInputAttribute("BITANGENT",	3, EFRIAttributeType::Float, EFRIBool::False, 60, 36),
		FRIInputAttribute("TEXCOORD",		3, EFRIAttributeType::Float, EFRIBool::False, 60, 48)
	};

	vertexDecl = cmdList.GetDynamic()->CreateVertexDeclaration({ FRIInputDesc(DeclCompArray, 0) }, signatureShader);

	FAssetManager Content;
	Content.RenderContext = friContext;
	Content.Connect("./Assets/");

	gameTextures = new GameTextureArray(cmdList, Content);

	ShaderLibrary lib = Content.Load<ShaderLibrary>("Shaders/mc.fslib");
	chunkShader = cmdList.GetDynamic()->CreateShaderPipeline(lib.Modules["ChunkGeometry"]);
	chunkShaderWater = cmdList.GetDynamic()->CreateShaderPipeline(lib.Modules["ChunkGeometryWater"]);

	auto& libmod = lib.Modules["Grass"];
	chunkShaderGrass = cmdList.GetDynamic()->CreateShaderPipeline(libmod);

	waveSettingsBuffer.GPU = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, 16));

	BitTexture bitTexture = Content.Load<BitTexture>("Materials/mc/grass_displ.flmt");
	windDisplacement = cmdList.GetDynamic()->CreateTexture2D(256, 256, 0, EFRITextureFormat::RGBA8UNORM, FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::UnsignedByte), bitTexture.DataDescriptor);

}

void WorldRenderer::AddToCmdList(FRICommandList& cmdList, GRenderMode renderMode)
{
	if (renderMode == GRenderMode::Material)
	{
		cmdList.SetShaderPipeline(chunkShader);
		cmdList.SetShaderSampler(FRISampler(0, gameTextures->friTexture));
	}
	else if (renderMode == GRenderMode::Transluscent)
	{
		cmdList.SetShaderPipeline(chunkShaderWater);
		cmdList.StageResourcesLambda(waveSettingsBuffer, [&](FRIMemoryMap& memory)
			{
				auto time = FTime::GetTimestamp();
				memory << time.GetSeconds();
				memory << time.GetSeconds();
				memory << time.GetSeconds();
				memory << time.GetSeconds();
			});

		cmdList.SetShaderUniformBuffer(12, waveSettingsBuffer.GPU);
	}

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
				mesh->RenderWater(cmdList);
			}
			else
			{
				mesh->RenderStatic(cmdList);
			}
		}
	}

	if (renderMode != GRenderMode::Transluscent)
	{
		cmdList.SetShaderPipeline(chunkShaderGrass);
		cmdList.SetShaderSampler(FRISampler(0, gameTextures->friTexture));
		cmdList.SetShaderSampler(FRISampler(1, windDisplacement));


		cmdList.StageResourcesLambda(waveSettingsBuffer, [&](FRIMemoryMap& memory)
			{
				auto time = FTime::GetTimestamp();
				memory << time.GetSeconds();
				memory << time.GetSeconds();
				memory << time.GetSeconds();
				memory << time.GetSeconds();
			});

		cmdList.SetShaderUniformBuffer(12, waveSettingsBuffer.GPU);

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
	}
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

	FRICommandList cmdList(FriContext->GetFRIDynamic());
	FArray<IVector2> iTaskDeleteQueue;

	for (auto& [chunkCoord, future] : tasks)
	{
		if (future->_Is_ready())
		{
			auto desc = future->get();
			desc.mesh->StageGeometry(cmdList, desc, vertexDecl);
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