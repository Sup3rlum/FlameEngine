#pragma once

#include "ModelMesh.h"

class DELUSION_DLL Model
{
public:
	vector<ModelMesh>* children;
	mat4x4 World;

	void Render();
};

