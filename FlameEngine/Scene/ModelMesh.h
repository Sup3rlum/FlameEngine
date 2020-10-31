#pragma once

#include "../dll/nchfx.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/Vertex.h"
#include "Material.h"

using namespace glm;

EXPORT_CLASS ModelMesh
{
public:
	vector<ModelMesh> children;

	Matrix4 World;
	VertexBuffer _vbo;
	Material _material;

	int ID;

	ModelMesh(int id);

	void Render();
};

