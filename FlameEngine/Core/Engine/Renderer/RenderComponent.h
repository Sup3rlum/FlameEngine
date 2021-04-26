#pragma once

#include "Core/Engine/FlameRI/FRIResource.h"
#include "../GameSystem/EntityComponent/EntityComponent.h"
#include "../GameSystem/Material/MaterialComponent.h"

struct RenderComponent
{
	FResourceVertexDeclaration VertexDeclaration;
	FResourceVertexBuffer* VertexBuffer;
	FResourceIndexBuffer* IndexBuffer;

	RenderComponent(FResourceVertexBuffer* vbuff, FResourceIndexBuffer* ibuff, FResourceVertexDeclaration vdecl ) : 
		VertexBuffer(vbuff), 
		IndexBuffer(ibuff), 
		VertexDeclaration(vdecl)
	{
	
		
	}

};
