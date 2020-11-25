#pragma once

#include "../../dll/nchfx.h"
#include "../../Graphics/VertexBuffer.h"
#include "../../Graphics/Vertex.h"
#include "../../Graphics/ShaderDefinitions/Shader.h"
#include "../Common/Material.h"
#include "Camera.h"

EXPORT_CLASS Scene;

EXPORT_CLASS Model
{
public:

	fMatrix4 World;

	VertexBuffer _vbo;
	Shader* _shader;


	Material* _material;

	void Render(fMatrix4 Transform, Scene* _cam);
};

