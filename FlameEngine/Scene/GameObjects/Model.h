#pragma once

#include "../../dll/nchfx.h"
#include "../../Graphics/VertexBuffer.h"
#include "../../Graphics/Vertex.h"
#include "../../Graphics/Shader.h"
#include "../Material.h"
#include "Camera.h"

EXPORT_CLASS Scene;

EXPORT_CLASS Model
{
public:

	fMatrix4 World;

	VertexBuffer _vbo;
	Material* _material;
	Shader* _shader;
	Shader* _depthShader;


	void Render(fMatrix4 Transform, Scene* _cam);
	void RenderDepth(fMatrix4 Transform, Scene* _cam);
};

