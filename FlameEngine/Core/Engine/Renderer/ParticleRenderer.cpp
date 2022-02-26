#include "ParticleRenderer.h"



ParticleRenderer::ParticleRenderer(FRIContext* FriContext, const ShaderLibraryModule& libraryModule) :
	FriContext(FriContext),
	VertexBuffer(0),
	IndexBuffer(0)
{
	FRICommandList cmdList(FriContext->GetFRIDynamic());

	ShaderPipeline = cmdList.GetDynamic()->CreateShaderPipeline(libraryModule);


	BlendState = cmdList.GetDynamic()->CreateBlendState(EFRIAlphaBlend::Src, EFRIAlphaBlend::OneMinusSrc);
	RasterizerState = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::Front, EFRIFillMode::Solid);
	DepthStencilState = cmdList.GetDynamic()->CreateDepthStencilState(EFRIBool::True, EFRIBool::False);
}

void ParticleRenderer::SetInstanceMesh(InstanceMesh mesh)
{
	assert(mesh.VertexBuffer != NULL);
	assert(mesh.IndexBuffer != NULL);

	VertexBuffer = mesh.VertexBuffer;
	IndexBuffer = mesh.IndexBuffer;
}


void ParticleRenderer::Prepare(FRICommandList& cmdList)
{
	for (int i = 0; i < 8 && i < Samplers.Length(); i++)
	{
		cmdList.SetShaderSampler(Samplers[i]);
	}
}

void ParticleRenderer::RenderSystem(FRICommandList& cmdList, ParticleSystemBase* pSystem)
{

	cmdList.SetBlendState(BlendState);
	cmdList.SetRasterizerState(RasterizerState);
	cmdList.SetDepthStencilState(DepthStencilState);

	cmdList.SetShaderPipeline(ShaderPipeline);
	uint32 InstanceCount = pSystem->GetStageCount();

	cmdList.SetInstancedGeometrySource(VertexBuffer, pSystem->GetInstanceBuffer());
	cmdList.DrawInstancesIndexed(EFRIPrimitiveType::Triangles, IndexBuffer->IndexCount, InstanceCount, EFRIIndexType::UInt32, IndexBuffer);
}