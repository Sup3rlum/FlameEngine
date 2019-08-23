#pragma once

#include "nchfx.h"
#include "VertexBuffer.h"

using namespace glm;

class DELUSION_DLL ModelMesh
{
public:
	vector<ModelMesh>* children;

	mat4x4 World;
	VertexBuffer* _vbo;

	void Render();
};

