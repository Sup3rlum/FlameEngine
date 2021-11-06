#pragma once

#include "RenderModule.h"


#include "../GameSystem/Environment/ParticleSystem.h"
#include "../GameSystem/Environment/InstanceMesh.h"

EXPORT(class, ParticleRenderer)
{
public:

	ParticleRenderer(FRIContext* FriContext, const ShaderLibraryModule& libraryModule);

	void RenderSystem(FRICommandList& cmdList, ParticleSystemBase* system);
	void SetInstanceMesh(InstanceMesh mesh);
	void Prepare(FRICommandList& cmdList);


	FRIVertexBuffer* VertexBuffer;
	FRIIndexBuffer* IndexBuffer;

	FRIShaderPipeline* ShaderPipeline;

	FRIContext* FriContext;

	FRIRasterizerState* RasterizerState;
	FRIDepthStencilState* DepthStencilState;
	FRIBlendState* BlendState;

	FArray<FUniformSampler> Samplers;




};