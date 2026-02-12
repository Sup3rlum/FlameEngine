#include "Mesh.h"
#include "Core/Engine/Renderer/Common/Geometry/VertexComponent.h"

void Mesh::AddToRenderList(FRICommandList& cmdList, EFRIPrimitiveType topology, FRIInstanceBuffer* instanceBuffer, uint32 InstanceCount) const
{
	cmdList.SetGeometrySource(VertexBuffer, IndexBuffer, instanceBuffer);

	if (!!instanceBuffer && HasInstanceAttributes)
	{
		cmdList.DrawInstancesIndexed(
			topology,
			IndexBuffer->IndexCount,
			InstanceCount,
			EFRIIndexType::UInt32);
	}
	else
	{
		cmdList.DrawPrimitivesIndexed(
			topology,
			IndexBuffer->IndexCount,
			EFRIIndexType::UInt32);

	}
}
/*
void Mesh::SetInstanceAttributes(
	FRICommandList& cmdList, 
	const FArray<FRIInputAttribute>& attributes,
	FRIVertexShader* signatureShader)
{
	/*FArray<FRIInputDesc> LayoutDesc =
	{
		FRIInputDesc(InputLayouts::StaticLit,	0),
		FRIInputDesc(attributes,				1)
	};

	auto inputLayout = cmdList.GetDynamic()->CreateInputLayout(LayoutDesc, signatureShader);
	cmdList.GetDynamic()->AttachInputLayout(this->VertexBuffer, inputLayout);

	if (inputLayout) 
		HasInstanceAttributes = true;
}*/

void Model::AddToRenderList(FRICommandList& cmdList, FRIInstanceBuffer* instanceBuffer, uint32 InstanceCount) const
{
	EFRIPrimitiveType primitiveTopology = EFRIPrimitiveType::Triangles;

	if (DetailMode == Detail::Tessellation &&
		TessellationMaxLevel > 0)
	{
		primitiveTopology = EFRIPrimitiveType::ControlPoint3;
	}

	Mesh.AddToRenderList(cmdList,
		primitiveTopology,
		instanceBuffer,
		InstanceCount);
}


/* --------------------------- Skinned Model -------------------------------*/

void RiggedMesh::AddToRenderList(FRICommandList& cmdList, EFRIPrimitiveType topology, FRIInstanceBuffer* instanceBuffer, uint32 InstanceCount) const
{
	cmdList.SetGeometrySource(VertexBuffer, IndexBuffer, instanceBuffer);

	if (!!instanceBuffer && HasInstanceAttributes)
	{
		cmdList.DrawInstancesIndexed(
			topology,
			IndexBuffer->IndexCount,
			InstanceCount,
			EFRIIndexType::UInt32);
	}
	else
	{
		cmdList.DrawPrimitivesIndexed(
			topology,
			IndexBuffer->IndexCount,
			EFRIIndexType::UInt32);

	}
}
/*
void RiggedMesh::SetInstanceAttributes(
	FRICommandList& cmdList,
	const FArray<FRIInputAttribute>& attributes,
	FRIVertexShader* signatureShader)
{
	FArray<FRIInputDesc> LayoutDesc =
	{
		FRIInputDesc(InputLayouts::StaticLit,	0),
		FRIInputDesc(attributes,				1)
	};

	auto inputLayout = cmdList.GetDynamic()->CreateInputLayout(LayoutDesc, signatureShader);
	cmdList.GetDynamic()->AttachInputLayout(this->VertexBuffer, inputLayout);

	if (inputLayout)
		HasInstanceAttributes = true;
}*/

void RiggedModel::AddToRenderList(FRICommandList& cmdList, FRIInstanceBuffer* instanceBuffer, uint32 InstanceCount) const
{
	EFRIPrimitiveType primitiveTopology = EFRIPrimitiveType::Triangles;

	if (DetailMode == Detail::Tessellation &&
		TessellationMaxLevel> 0)
	{
		primitiveTopology = EFRIPrimitiveType::ControlPoint3;
	}

	Mesh.AddToRenderList(cmdList,
		primitiveTopology,
		instanceBuffer,
		InstanceCount);
}