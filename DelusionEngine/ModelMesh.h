#pragma once

#include "nchfx.h"
#include "VertexBuffer.h"

using namespace glm;

EXPORT_CLASS ModelMesh
{
public:
	vector<ModelMesh>* children;

	Matrix4 World;
	VertexBuffer* _vbo;

	void Render();
};

