#pragma once


#include "../../dll/nchfx.h"

#include "../../Graphics/ShaderDefinitions/Shader.h"
#include "RenderableObject.h"
#include "UpdatableObject.h"
#include "../Physics/PhysXService.h"


EXPORT_CLASS Scene;

EXPORT_CLASS Actor : public RenderableObject, public UpdatableObject
{
public:
	
	uint32_t id;

	Actor();

	fVector3 Position;
	fQuaternion Rotation;
	fVector3 Scale;


	Scene* _scene;


	PxRigidActor* pPxActor;

};

