#include "Mesh.h"
#include "Core/Engine/Renderer/Common/Geometry/VertexComponent.h"

void Mesh::AddToRenderList(FRICommandList& cmdList, EFRIPrimitiveType topology) const
{
	cmdList.SetGeometrySource(VertexBuffer);
	cmdList.DrawPrimitivesIndexed(
		topology,
		IndexBuffer->IndexCount,
		EFRIIndexType::UInt32,
		IndexBuffer);
}

void Mesh::AddToRenderListInstanced(FRICommandList& cmdList, EFRIPrimitiveType topology, FRIInstanceBuffer* instanceBuffer, uint32 InstanceCount) const
{
	cmdList.SetInstancedGeometrySource(VertexBuffer, instanceBuffer);
	cmdList.DrawInstancesIndexed(
		topology,
		IndexBuffer->IndexCount,
		InstanceCount,
		EFRIIndexType::UInt32,
		IndexBuffer);

}


void Mesh::SetInstanceAttributes(
	FRICommandList& cmdList, 
	const FArray<FRIInputAttribute>& attributes,
	FRIVertexShader* signatureShader)
{
	FArray<FRIInputDesc> LayoutDesc =
	{
		FRIInputDesc(InputLayouts::StaticLit,	0),
		FRIInputDesc(attributes,				1)
	};

	auto vDecl = cmdList.GetDynamic()->CreateVertexDeclaration(LayoutDesc, signatureShader);
	cmdList.GetDynamic()->AttachVertexDeclaration(this->VertexBuffer, vDecl);

}

void Model::AddToRenderList(FRICommandList& cmdList) const
{
	if (DetailMode == Detail::Tessellation && 
		TessellationMaxLevel > 0)
	{
		Mesh.AddToRenderList(cmdList, 
			EFRIPrimitiveType::ControlPoint3);
	}
	else
	{
		Mesh.AddToRenderList(cmdList,
			EFRIPrimitiveType::Triangles);
	}
}

void Model::AddToRenderListInstanced(FRICommandList& cmdList, FRIInstanceBuffer* instanceBuffer, uint32 InstanceCount) const
{
	if (DetailMode == Detail::Tessellation &&
		TessellationMaxLevel> 0)
	{
		Mesh.AddToRenderListInstanced(cmdList,
			EFRIPrimitiveType::ControlPoint3,
			instanceBuffer,
			InstanceCount);

	}
	else
	{
		Mesh.AddToRenderListInstanced(cmdList,
			EFRIPrimitiveType::Triangles,
			instanceBuffer,
			InstanceCount);
	}
}