#pragma once


#include "EntityComponent/EntityComponent.h"
#include "Material/MaterialComponent.h"
#include "../PCI/PCI.h"
#include "../FlameRI/ShaderLibrary/ShaderLibrary.h"

EXPORT(struct, Mesh)
{
	FRIVertexBuffer* VertexBuffer;
	FRIIndexBuffer* IndexBuffer;

	Mesh(FRIVertexBuffer* vbuff, FRIIndexBuffer* ibuff) :
		VertexBuffer(vbuff),
		IndexBuffer(ibuff)
	{}

	Mesh() :
		VertexBuffer(NULL),
		IndexBuffer(NULL)
	{}

	Mesh(const Mesh& other) :
		VertexBuffer(other.VertexBuffer),
		IndexBuffer(other.IndexBuffer)
	{
	}
	void AddToRenderList(FRICommandList& cmdList, EFRIPrimitiveType topology) const;
	void AddToRenderListInstanced(FRICommandList& cmdList, EFRIPrimitiveType topology, FRIInstanceBuffer* instanceBuffer, uint32 NumInstances) const;

	void SetInstanceAttributes(FRICommandList& cmdList, const FArray<FRIInputAttribute>& attributes, FRIVertexShader* signatureShader);
};

EXPORT(struct, Model) : public IProperties
{
	enum class Detail
	{
		None,
		POM,
		Tessellation
	};

	PropertyEnum(Detail, DetailMode, Detail::None)
	PropertyInt(DetailScale, 0)
	PropertyInt(TessellationMinLevel, 0)
	PropertyInt(TessellationMaxLevel, 0)

	void AddToRenderList(FRICommandList& cmdList) const;
	void AddToRenderListInstanced(FRICommandList& cmdList, FRIInstanceBuffer* instanceBuffer, uint32 NumInstances) const;

	Mesh Mesh;
	Material Material;
};