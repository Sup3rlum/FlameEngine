#pragma once


#include "EntityComponent/EntityComponent.h"
#include "Core/Engine/FlameRI/FRIResource.h"

#include "Animation/SkeletalComponent.h"


struct SkinnedMeshComponent
{
	FRIVertexBuffer* VertexBuffer;
	FRIIndexBuffer* IndexBuffer;
	//FRIVertexDeclaration VertexDeclaration;

	Skeleton MeshSkeleton;

	SkinnedMeshComponent(FRIVertexBuffer* vbuff, FRIIndexBuffer* ibuff, Skeleton Skeleton) :
		VertexBuffer(vbuff),
		IndexBuffer(ibuff),
		//VertexDeclaration(vdecl),
		MeshSkeleton(Skeleton)
	{}

	SkinnedMeshComponent() :
		VertexBuffer(NULL),
		IndexBuffer(NULL),
		//VertexDeclaration(NULL),
		MeshSkeleton(NULL, 0)
	{}

};