#pragma once

#include "../dll/nchfx.h"
#include "../Graphics/VertexBuffer.h"

using namespace glm;

EXPORT_CLASS ModelMesh
{
public:
	vector<ModelMesh>* children;

	Matrix4 World;
	VertexBuffer* _vbo;

	void Render();
};

