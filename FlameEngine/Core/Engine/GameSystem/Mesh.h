#pragma once


#include "EntityComponent/EntityComponent.h"
#include "Material/Material.h"
#include "../PCI/PCI.h"
#include "../FlameRI/ShaderLibrary/ShaderLibrary.h"
#include "Animation/SkeletalComponent.h"

EXPORT(struct, Mesh)
{
	FRIVertexBuffer* VertexBuffer;
	FRIIndexBuffer* IndexBuffer;
	bool HasInstanceAttributes = false;

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
	void AddToRenderList(FRICommandList& cmdList, EFRIPrimitiveType topology, FRIInstanceBuffer* instanceBuffer = nullptr, uint32 NumInstances = 0) const;
	//void SetInstanceAttributes(FRICommandList& cmdList, const FArray<FRIInputAttribute>& attributes, FRIVertexShader* signatureShader);
};

EXPORT(struct, Model) : public IProperties
{
	enum class Detail
	{
		None,
		POM,
		Tessellation
	};

	PropertyBool(EnableShadows, true)
	PropertyEnum(Detail, DetailMode, Detail::None)
	PropertyInt(DetailScale, 0)
	PropertyInt(TessellationMinLevel, 0)
	PropertyInt(TessellationMaxLevel, 0)

	void AddToRenderList(FRICommandList& cmdList, FRIInstanceBuffer* instanceBuffer = nullptr, uint32 NumInstances = 0) const;

	Mesh Mesh;
	Material Material;
};


EXPORT(struct, RiggedMesh)
{
	FRIVertexBuffer* VertexBuffer;
	FRIIndexBuffer* IndexBuffer;

	bool HasInstanceAttributes = false;
	Skeleton MeshSkeleton;

	RiggedMesh(FRIVertexBuffer* vbuff, FRIIndexBuffer* ibuff, Skeleton Skeleton) :
		VertexBuffer(vbuff),
		IndexBuffer(ibuff),
		MeshSkeleton(Skeleton)
	{}

	RiggedMesh() :
		VertexBuffer(NULL),
		IndexBuffer(NULL),
		MeshSkeleton(NULL, 0)
	{}

	RiggedMesh(const RiggedMesh & other) :
		VertexBuffer(other.VertexBuffer),
		IndexBuffer(other.IndexBuffer),
		MeshSkeleton(other.MeshSkeleton)
	{
	}
	void AddToRenderList(FRICommandList & cmdList, EFRIPrimitiveType topology, FRIInstanceBuffer * instanceBuffer = nullptr, uint32 NumInstances = 0) const;
	//void SetInstanceAttributes(FRICommandList & cmdList, const FArray<FRIInputAttribute>&attributes, FRIVertexShader * signatureShader);
};

EXPORT(struct, RiggedModel) : public IProperties
{
	enum class Detail
	{
		None,
		POM,
		Tessellation
	};

	PropertyBool(EnableShadows, true)
	PropertyEnum(Detail, DetailMode, Detail::None)
	PropertyInt(DetailScale, 0)
	PropertyInt(TessellationMinLevel, 0)
	PropertyInt(TessellationMaxLevel, 0)

	void AddToRenderList(FRICommandList& cmdList, FRIInstanceBuffer* instanceBuffer = nullptr, uint32 NumInstances = 0) const;

	RiggedMesh Mesh;
	Material Material;
};