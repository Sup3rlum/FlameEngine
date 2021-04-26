#pragma once


#include "EntityComponent/EntityComponent.h"
#include "Core/Engine/FlameRI/FRIResource.h"


struct MeshComponent
{
	FResourceVertexBuffer* VertexBuffer;
	FResourceIndexBuffer* IndexBuffer;
	FResourceVertexDeclaration VertexDeclaration;

	MeshComponent(FResourceVertexBuffer* vbuff, FResourceIndexBuffer* ibuff, FResourceVertexDeclaration vdecl) :
		VertexBuffer(vbuff),
		IndexBuffer(ibuff),
		VertexDeclaration(vdecl)
	{}

	MeshComponent() :
		VertexBuffer(NULL),
		IndexBuffer(NULL),
		VertexDeclaration(NULL)
	{}

};