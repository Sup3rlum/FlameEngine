#pragma once

#include "ModelMesh.h"
#include "SceneGeometry.h"

EXPORT_CLASS Model : SceneGeometry
{
public:
	vector<ModelMesh>* children;
	Matrix4 World;

	void Render();
};

