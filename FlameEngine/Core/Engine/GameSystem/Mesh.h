#pragma once


#include "EntityComponent/EntityComponent.h"
#include "Core/Engine/FlameRI/FRIResource.h"

struct Mesh
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

};