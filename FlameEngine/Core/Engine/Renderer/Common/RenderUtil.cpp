#include "RenderUtil.h"

void FRenderUtil::CreateResources(FRIDynamicAllocator* Allocator)
{
	VertexBuffer = Allocator->CreateVertexBuffer(4, sizeof(FVertex_PositionTexture), EFRIAccess::None, EFRIUsage::Default, FRICreationDescriptor(quadVertices.Begin(), quadVertices.ByteSize()));
	IndexBuffer = Allocator->CreateIndexBuffer(6, EFRIAccess::None, EFRIUsage::Default, FRICreationDescriptor(quadIndices.Begin(), quadIndices.ByteSize()));
}


void FRenderUtil::DrawScreenQuad(FRICommandList& cmdList)
{
	cmdList.SetGeometrySource(VertexBuffer, IndexBuffer);
	cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, 6, EFRIIndexType::UInt32);
}