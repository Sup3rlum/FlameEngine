#pragma once

#include "../../dll/nchfx.h"
#include "../../Graphics/VertexBuffer.h"
#include "../../Graphics/Vertex.h"
#include "../../Graphics/Shader.h"
#include "../Material.h"
#include "Camera.h"


EXPORT_CLASS Model
{
public:

	fMatrix4 World;

	VertexBuffer _vbo;
	Material* _material;
	Shader* _shader;

	void Render(fMatrix4 Transform, Camera* _cam);
};

