#pragma once

#include "RenderModule.h"


#include "../GameSystem/Environment/ParticleSystem.h"
#include "../GameSystem/Environment/InstanceMesh.h"

EXPORT(class, ParticleRenderer)
{
public:

	ParticleRenderer(FRIContext* FriContext, const ShaderLibraryModule& libraryModule, InstanceMesh mesh);

	void RenderSystem(FRICommandList& cmdList, ParticleSystemBase* system);

	FRIVertexBuffer* VertexBuffer;
	FRIIndexBuffer* IndexBuffer;

	FRIShaderPipeline* ShaderPipeline;

	FRIContext* FriContext;
};