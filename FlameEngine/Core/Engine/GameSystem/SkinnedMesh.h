#pragma once


#include "EntityComponent/EntityComponent.h"
#include "Core/Engine/FlameRI/FRIResource.h"

#include "Animation/SkeletalComponent.h"


struct SkinnedMesh
{
	FRIVertexBuffer* VertexBuffer;
	FRIIndexBuffer* IndexBuffer;
	//FRIVertexDeclaration VertexDeclaration;

	Skeleton MeshSkeleton;

	SkinnedMesh(FRIVertexBuffer* vbuff, FRIIndexBuffer* ibuff, Skeleton Skeleton) :
		VertexBuffer(vbuff),
		IndexBuffer(ibuff),
		//VertexDeclaration(vdecl),
		MeshSkeleton(Skeleton)
	{}

	SkinnedMesh() :
		VertexBuffer(NULL),
		IndexBuffer(NULL),
		//VertexDeclaration(NULL),
		MeshSkeleton(NULL, 0)
	{}

};