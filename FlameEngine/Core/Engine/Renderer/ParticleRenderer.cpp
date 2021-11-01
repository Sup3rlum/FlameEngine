#include "ParticleRenderer.h"



ParticleRenderer::ParticleRenderer(FRIContext* FriContext, const ShaderLibraryModule& libraryModule, InstanceMesh mesh) :
	FriContext(FriContext)
{
	assert(mesh.VertexBuffer != NULL);
	assert(mesh.IndexBuffer != NULL);

	FRICommandList cmdList(FriContext->GetFRIDynamic());

	ShaderPipeline = cmdList.GetDynamic()->CreateShaderPipeline(libraryModule);

	VertexBuffer = mesh.VertexBuffer;
	IndexBuffer = mesh.IndexBuffer;

}

void ParticleRenderer::RenderSystem(FRICommandList& cmdList, ParticleSystemBase* pSystem)
{
	cmdList.SetShaderPipeline(ShaderPipeline);


	uint32 InstanceCount = pSystem->GetStageCount();

	cmdList.SetInstancedGeometrySource(VertexBuffer, pSystem->GetInstanceBuffer());
	cmdList.DrawInstancesIndexed(EFRIPrimitiveType::Triangles, IndexBuffer->IndexCount, InstanceCount, EFRIIndexType::UInt32, IndexBuffer);
}