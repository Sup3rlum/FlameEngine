#pragma once


#include "EntityComponent/EntityComponent.h"
#include "Core/Engine/FlameRI/FRIResource.h"

#include "Animation/SkeletalComponent.h"


struct SkinnedMeshComponent
{
	FResourceVertexBuffer* VertexBuffer;
	FResourceIndexBuffer* IndexBuffer;
	FResourceVertexDeclaration VertexDeclaration;

	SkeletalComponent Skeleton;

	SkinnedMeshComponent(FResourceVertexBuffer* vbuff, FResourceIndexBuffer* ibuff, FResourceVertexDeclaration vdecl, SkeletalComponent Skeleton) :
		VertexBuffer(vbuff),
		IndexBuffer(ibuff),
		VertexDeclaration(vdecl),
		Skeleton(Skeleton)
	{}

	SkinnedMeshComponent() :
		VertexBuffer(NULL),
		IndexBuffer(NULL),
		VertexDeclaration(NULL),
		Skeleton(NULL, 0)
	{}

};