#pragma once

#include "../../dll/nchfx.h"
#include "../Common/VertexBuffer.h"
#include "../Common/Vertex.h"
#include "../ShaderDefinitions/Shader.h"
#include "../Common/Material.h"

EXPORT_FORWARD_DECL(class,  Scene);

EXPORT(class,  ModelBase)
{
public:

	fMatrix4 World;

	VertexBuffer _vbo;
	Shader* _shader;


	Material* _material;

	void Render(fMatrix4 Transform, Scene* _cam);
};

