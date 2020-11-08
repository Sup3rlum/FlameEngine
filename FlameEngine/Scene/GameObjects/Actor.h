#pragma once


#include "../../dll/nchfx.h"

#include "../../Graphics/Shader.h"
#include "RenderableObject.h"


using namespace glm;

EXPORT_CLASS Scene;

EXPORT_CLASS Actor : public RenderableObject
{
public:
	
	std::string id;

	Actor();

	fVector3 Position;
	Quaternion Rotation;
	fVector3 Scale;

	Scene* _scene;

};

