#pragma once


#include "../../../dll/nchfx.h"
#include "../../Common/VertexBuffer.h"
#include "../../Common/Vertex.h"
#include "../../ShaderDefinitions/Program.h"
#include "../../MaterialSystem/Baked/BakedMaterial.h"

EXPORT_FORWARD_DECL(class, Scene);


EXPORT(class, ModelMeshBase)
{
public:

	virtual void Render();
	virtual void Update();

	FMatrix4 LocalTransform;

	VertexBuffer* mVertexBuffer;
	Program* pRenderingProgram;


	BakedMaterial* mMeshMaterial;

};

