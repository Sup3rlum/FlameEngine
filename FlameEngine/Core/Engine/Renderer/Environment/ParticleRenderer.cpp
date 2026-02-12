#include "ParticleRenderer.h"



void ParticleRenderer::CreateResources(FRIContext* FriContext)
{
	/*
	this->FriContext = FriContext;

	FRICommandList cmdList(FriContext->GetFRIDynamic());

	BlendState = cmdList.GetDynamic()->CreateBlendState(EFRIBlend::Src, EFRIBlend::OneMinusSrc, EFRIBlend::Src, EFRIBlend::Dst);
	RasterizerState = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);
	DepthStencilState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::True, EFRIBool::False);*/
}


void ParticleRenderer::RenderSystem(FRICommandList& cmdList, IParticleSystem* pSystem)
{
	/*
	cmdList.SetBlendState(BlendState);
	cmdList.SetRasterizerState(RasterizerState);
	cmdList.SetDepthStencilState(DepthStencilState);

	cmdList.SetShaderPipeline(pSystem->Shader);

	for (auto sampler : FRange(pSystem->Samplers.Length()))
	{
		cmdList.SetShaderResource(sampler, pSystem->Samplers[sampler]);
	}

	pSystem->ParticleMesh.AddToRenderList(cmdList, EFRIPrimitiveType::Triangles, pSystem->GetInstanceBuffer(), pSystem->GetStageCount());*/
}