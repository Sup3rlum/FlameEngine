#pragma once


#include <FlameEngine/Core/Engine/GameSystem/RenderObject.h>
#include <FlameEngine/Core/Framework/IO/Stream.h>
#include <FlameEngine/Core/Engine/ContentSystem/Client/LocalAssetManager.h>
#include <FlameEngine/Core/Engine/ContentSystem/ImportScripts/Common.h>

#include "ChunkMesh.h"
#include "../World/WorldObserver.h"
#include "GameTextureArray.h"

#include "../World/ctpl.h"

struct WorldRenderer : public RenderList, public WorldObserver
{
	typedef std::future<ChunkMeshDescription> ChunkMeshFuture;

	
	ctpl::thread_pool threadPool;
	FHashMap<IVector2, ChunkMesh*, ChunkIDHasher> meshes;
	FRIInputLayout* vertexDecl = NULL;
	FRIContext* FriContext;

	FHashMap<IVector2, ChunkMeshFuture*, ChunkIDHasher> tasks;
	FRITexture2D* windDisplacement;

	GameTextureArray* gameTextures;
	FRIPipelineStateObject* chunkShader;
	FRIPipelineStateObject* chunkShaderWater;
	FRIPipelineStateObject* chunkShaderGrass;
	FRIStageBuffer waveSettingsBuffer;
	FRISamplerState* sampler;

	void BlockChanged(Block oldBlock, Block newBlock, IVector3 globalPos) override;
	void ChunkScheduledUpdate(IVector2 chunkCoord, bool threadpool) override;
	void WorldTick() override;
	void FlushFutures();
	FRIPipelineStateObject* CreateChunkShaderPipeline(FRIDynamicAllocator* Allocator, const ShaderLibraryModule& Shaders);

	WorldRenderer(FRIContext* friContext, World* world);
	void AddToCmdList(FRICommandList& cmdList, GRenderMode mode) override;
};
