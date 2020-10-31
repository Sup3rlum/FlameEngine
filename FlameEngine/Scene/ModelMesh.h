#pragma once

#include "../dll/nchfx.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/Vertex.h"
#include "../Graphics/Shader.h"
#include "Material.h"
#include "Camera.h"

using namespace glm;

EXPORT_CLASS ModelMesh
{
public:
	vector<ModelMesh> children;

	Matrix4 World;
	VertexBuffer _vbo;
	Material* _material;
	Shader* _shader;

	int ID;

	ModelMesh(int id);

	void Render(Matrix4 Transform, Camera* cam);
};

