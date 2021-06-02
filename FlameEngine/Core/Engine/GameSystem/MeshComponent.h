#pragma once


#include "EntityComponent/EntityComponent.h"
#include "Core/Engine/FlameRI/FRIResource.h"


struct MeshComponent
{
	FRIVertexBuffer* VertexBuffer;
	FRIIndexBuffer* IndexBuffer;
	//FRIVertexDeclaration VertexDeclaration;

	MeshComponent(FRIVertexBuffer* vbuff, FRIIndexBuffer* ibuff) :
		VertexBuffer(vbuff),
		IndexBuffer(ibuff)
		//VertexDeclaration(vdecl)
	{}

	MeshComponent() :
		VertexBuffer(NULL),
		IndexBuffer(NULL)
		//VertexDeclaration(NULL)
	{}

};