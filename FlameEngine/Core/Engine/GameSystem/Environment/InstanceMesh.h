#pragma once

#include "core/Engine/FlameRI/FRI.h"


struct InstanceMesh
{
	FRIVertexBuffer* VertexBuffer;
	FRIIndexBuffer* IndexBuffer;


	InstanceMesh(FRIVertexBuffer* vbuff, FRIIndexBuffer* ibuff) :
		VertexBuffer(vbuff),
		IndexBuffer(ibuff)
	{}

	InstanceMesh() :
		VertexBuffer(NULL),
		IndexBuffer(NULL)
	{}
};