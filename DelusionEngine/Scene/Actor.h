#pragma once


#include "../dll/nchfx.h"

#include "SceneGeometry.h"
#include "../Graphics/Shader.h"


using namespace glm;


EXPORT_CLASS Scene;

EXPORT_CLASS Actor
{
public:
	
	std::string id;

	Actor(Scene* sc);
	void Render();

	Vector3 Position;
	Quaternion Rotation;

	SceneGeometry* geometry;
	Scene* _scene;
};

