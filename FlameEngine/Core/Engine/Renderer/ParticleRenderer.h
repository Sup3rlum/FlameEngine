#pragma once

#include "RenderModule.h"


#include "../GameSystem/Environment/ParticleSystem.h"
#include "../GameSystem/Environment/InstanceMesh.h"

EXPORT(class, ParticleRenderer)
{
public:
	void CreateResources(FRIContext* FriContext);
	void RenderSystem(FRICommandList& cmdList, IParticleSystem* system);

	FRIContext* FriContext;

	FRIRasterizerState* RasterizerState;
	FRIDepthStencilState* DepthStencilState;
	FRIBlendState* BlendState;
};