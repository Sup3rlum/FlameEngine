#pragma once

#include "../dll/nchfx.h"

#include "SceneGeometry.h"
#include "../Graphics/Shader.h"

using namespace glm;




EXPORT_CLASS Actor
{
public:
	
	std::string id;

	Actor();
	void Render();

	SceneGeometry* geometry;
};

