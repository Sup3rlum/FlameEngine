#pragma once

#include "ModelMesh.h"

class DELUSION_DLL Model
{
public:
	vector<ModelMesh>* children;
	Matrix4 World;

	void Render();
};

