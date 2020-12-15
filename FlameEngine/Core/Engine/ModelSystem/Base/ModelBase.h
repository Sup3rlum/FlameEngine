#pragma once

#include "../../../dll/nchfx.h"
#include "../../Common/VertexBuffer.h"
#include "../../Common/Vertex.h"
#include "../../ShaderDefinitions/Shader.h"
#include "ModelMeshBase.h"



EXPORT(class,  ModelBase)
{
public:

	FMatrix4 World;


	virtual void Render(Program* pRenderingProgram, Scene* _cam);
	virtual void Update();
};

