#pragma once

#include "ModelMesh.h"

EXPORT_CLASS Model
{
public:
	vector<ModelMesh>* children;
	Matrix4 World;

	void Render();
};

