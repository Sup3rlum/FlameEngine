#pragma once


#include "../../dll/nchfx.h"

#include "../../Graphics/Shader.h"
#include "RenderableObject.h"
#include "UpdatableObject.h"



EXPORT_CLASS Scene;

EXPORT_CLASS Actor : public RenderableObject, public UpdatableObject
{
public:
	
	std::string id;

	Actor();

	fVector3 Position;
	fQuaternion Rotation;
	fVector3 Scale;


	Scene* _scene;

};

